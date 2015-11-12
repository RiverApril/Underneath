package takp.nedearb.underneath;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "UND-MainActivity";

    public GameSurfaceView gameSurfaceView;

    private final int KEY_UP = 0x103;
    private final int KEY_DOWN = 0x102;
    private final int KEY_LEFT = 0x104;
    private final int KEY_RIGHT = 0x105;
    private final int KEY_BACKSPACE = 0x107;
    private final int KEY_ENTER = 0x157;
    private final int KEY_EXIT = 0x169;
    private final int KEY_ESCAPE = 27;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        gameSurfaceView = (GameSurfaceView)findViewById(R.id.gameSurfaceView);
        gameSurfaceView.init(this);
        
        

        gameSurfaceView.addButton(this, R.id.buttonExam, (int) 'q');
        gameSurfaceView.addButton(this, R.id.buttonAutoWalk, (int)'e');
        gameSurfaceView.addButton(this, R.id.buttonInv, (int)'i');
        gameSurfaceView.addButton(this, R.id.buttonUp, KEY_UP);
        gameSurfaceView.addButton(this, R.id.buttonEscape, KEY_ESCAPE);

        gameSurfaceView.addButton(this, R.id.buttonUse, (int)'g');
        gameSurfaceView.addButton(this, R.id.buttonWait, (int)'w');
        gameSurfaceView.addButton(this, R.id.buttonLeft, KEY_LEFT);
        gameSurfaceView.addButton(this, R.id.buttonEnter, (int)'\n');
        gameSurfaceView.addButton(this, R.id.buttonRight, KEY_RIGHT);

        gameSurfaceView.addButton(this, R.id.buttonSecondAtk, (int)'G');
        gameSurfaceView.addButton(this, R.id.buttonHeal, (int)'h');
        gameSurfaceView.addButton(this, R.id.buttonSkills, (int)'s');
        gameSurfaceView.addButton(this, R.id.buttonDown, KEY_DOWN);
        gameSurfaceView.addButton(this, R.id.buttonAdjBord, (int)'\\');

        gameSurfaceView.addButton(this, R.id.buttonSortInv, (int)'s');
        gameSurfaceView.addButton(this, R.id.buttonTranItem, (int)'t');
        gameSurfaceView.addButton(this, R.id.buttonDropItem, (int)'d');
        gameSurfaceView.addButton(this, R.id.buttonEquipItem, (int)'e');
        Button keyboardButton = (Button)findViewById(R.id.buttonKeyboard);
        keyboardButton.setOnClickListener(gameSurfaceView);

        //
        gameSurfaceView.addButton(this, R.id.buttonTranStack, (int)'T');
        gameSurfaceView.addButton(this, R.id.buttonDropStack, (int)'D');
        gameSurfaceView.addButton(this, R.id.buttonEquipPrime, (int)'E');
        //

    }
}
