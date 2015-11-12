package takp.nedearb.underneath;

/**
 * Created by Nedearb on 11/9/15.
 */
public class GameThread extends Thread{

    private final GameSurfaceView gameSurfaceView;
    public boolean running = false;

    public GameThread(GameSurfaceView gameSurfaceView){
        this.gameSurfaceView = gameSurfaceView;
    }

    @Override
    public void run(){
        gameSurfaceView.threadRun(this);
    }
}
