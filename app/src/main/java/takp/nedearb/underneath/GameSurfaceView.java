package takp.nedearb.underneath;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.DrawFilter;
import android.graphics.Paint;
import android.graphics.PaintFlagsDrawFilter;
import android.graphics.Rect;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;

import java.util.HashMap;
import java.util.PriorityQueue;
import java.util.Random;

/**
 * Created by Nedearb on 11/9/15.
 */
public class GameSurfaceView extends SurfaceView implements SurfaceHolder.Callback, View.OnClickListener {

    public static final String TAG = "UND-GameSurfaceView";

    private static final int ERR = -1;

    private GameThread thread;

    public MainActivity mainActivity;
    private int tick = 0;
    private Bitmap fontBitmap;

    private int canvasWidth, canvasHeight;
    private int imageCellWidth, imageCellHeight;
    private int cellWidth, cellHeight;
    private int fontScale = 1;
    private int imageCharsPerLine = 16;
    private int imageLineCount = 6;
    private int bufferSize;
    private int bufferWidth, bufferHeight;
    private char[] charBuffer;
    private byte[] attrBuffer;
    private boolean bufferChanged = true;
    private boolean buffersSet = false;

    private boolean initalizedCpp = false;

    private Rect src, dst;

    private PriorityQueue<Integer> inputQueue = new PriorityQueue<>();

    private final int KEY_RESIZE = 0x200;

    public HashMap<Integer, Integer> buttonHashMap = new HashMap<>(); //<id, key>

    private final int A_COLOR = 0x03fe0000;

    private DrawFilter filter;
    private Paint paint;
    private Canvas canvas;
    private int fgColorCode;
    private int bgColorCode;

    public GameSurfaceView(Context context) {
        super(context);
    }

    public GameSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public GameSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public GameSurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    public void init(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
        thread = new GameThread(this);

        getHolder().addCallback(this);

        setFocusable(true);

        setWillNotDraw(false);

        fontBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.font);
        //fontBitmap = Bitmap.createScaledBitmap(fontBitmap, fontScale, fontScale, false);
        imageCellWidth = fontBitmap.getWidth() / imageCharsPerLine;
        imageCellHeight = fontBitmap.getHeight() / imageLineCount;
        cellWidth = imageCellWidth * fontScale;
        cellHeight = imageCellHeight * fontScale;

        Log.d(TAG, "Image Cell Size: "+imageCellWidth+", "+imageCellHeight);

        src = new Rect();
        dst = new Rect();
        
        paint = new Paint();
        paint.setAntiAlias(false);
        paint.setFilterBitmap(false);

        Rect windowRect = new Rect();

        this.getWindowVisibleDisplayFrame(windowRect);

        int hWindow = windowRect.bottom - windowRect.top;
        int wWindow = windowRect.right - windowRect.left;

        this.getHolder().setFixedSize(wWindow, hWindow / 2);

    }

    public void addButton(MainActivity activity, int id, int key){

        Button button = (Button)activity.findViewById(id);
        if(button == null){
            Log.e(TAG, "Button null");
            return;
        }
        button.setOnClickListener(this);
        buttonHashMap.put(id, key);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        if (!thread.running) {
            thread.running = true;
            thread.start();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (canvasWidth != width || canvasHeight != height) {
            Log.v(TAG, "Surface Changed, remaking buffers.");
            canvasWidth = width;
            canvasHeight = height;
            bufferWidth = canvasWidth / cellWidth;
            bufferHeight = canvasHeight / cellHeight;

            bufferSize = bufferWidth * bufferHeight;
            charBuffer = new char[bufferSize];
            attrBuffer = new byte[bufferSize];

            for (int i = 0; i < bufferSize; i++) {
                charBuffer[i] = ' ';
                attrBuffer[i] = 0;
            }
            Log.v(TAG, "Buffer Size: "+ bufferWidth +"x"+ bufferHeight +" = "+bufferSize);

            inputQueue.add(KEY_RESIZE);

            buffersSet = true;

            sendInfoToCpp(bufferWidth, bufferHeight);

        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        thread.running = false;
        boolean retry = true;
        while (retry) {
            try {
                thread.join();
                retry = false;
            } catch (InterruptedException e) {

            }
        }
    }

    @Override
    public void onDraw(Canvas canvas) {
    }

    private void draw(){

        tick++;
        if(bufferChanged) {
            canvas = getHolder().lockCanvas();
            if(canvas != null){
                //Log.d(TAG, "Buffer changed, drawing...");
                int pos, i, j, c, x, y;
                for (pos = 0; pos < bufferSize; pos++) {
                    c = Math.max(((int) charBuffer[pos]) - 32, 0);
                    i = (c % imageCharsPerLine) * imageCellWidth;
                    j = (c / imageCharsPerLine) * imageCellHeight;

                    x = (pos % bufferWidth) * cellWidth;
                    y = (pos / bufferWidth) * cellHeight;

                    //if(c != 0) {
                    //Log.d(TAG, "c:"+c+" i:"+i+" j:"+j);
                    //Log.d(TAG, "p:"+"x:"+x+" y:"+y);
                    //}

                    fgColorCode = ((attrBuffer[pos] & A_COLOR) >> 17) & 0xF;
                    bgColorCode = (((attrBuffer[pos] & A_COLOR) >> 17) >> 4) & 0xF;

                    src.set(i, j, i + imageCellWidth, j + imageCellHeight);
                    dst.set(x, y, x + cellWidth, y + cellHeight);

                    canvas.drawBitmap(fontBitmap, src, dst, paint);
                }
                bufferChanged = false;
                getHolder().unlockCanvasAndPost(canvas);
            }
        }
    }

    @Override
    public void onClick(View v) {
        if(buttonHashMap.containsKey(v.getId())) {
            inputQueue.add(buttonHashMap.get(v.getId()));
        }else if(v.getId() == R.id.buttonKeyboard){

        }
    }



    //NATIVE:

    static {
        System.loadLibrary("AndroidCursesInterface");
    }

    public native void initCpp();
    public native void updateCpp();
    public native void cleanupCpp();

    public native void sendInfoToCpp(int bufferWidth, int bufferHeight);


    public void update() {

    }

    public void refresh() {
        bufferChanged = true;
        postInvalidate();
        //Log.d(TAG, "bufferChanged=true and postInvalidate()");
    }

    public int getCode() {
        if(!inputQueue.isEmpty()){
            return inputQueue.poll();
        }
        return ERR;
    }

    public void setCharInBuffer(int pos, int c, int a){
        charBuffer[pos] = (char)c;
        attrBuffer[pos] = (byte)a;
        //Log.d(TAG, "Set Char at ["+pos+"] "+c+", "+a);
    }


    public void threadRun(GameThread threadInstance) {
        while(!buffersSet);
        if(!initalizedCpp){
            Log.d(TAG, "Before initCpp()");
            initCpp();
            initalizedCpp = true;
            Log.d(TAG, "After initCpp()");
        }else{
            Log.d(TAG, "Already ran initCpp()");
        }
        Log.d(TAG, "Starting updateCpp() loop");
        while(threadInstance.running){
            updateCpp();
            try {
                threadInstance.sleep(100);
            }catch(InterruptedException e){

            }
            draw();
        }
        Log.d(TAG, "After updateCpp() loop");
        //cleanupCpp();
    }
}
