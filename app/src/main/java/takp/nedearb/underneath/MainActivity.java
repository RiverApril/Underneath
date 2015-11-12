package takp.nedearb.underneath;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "UND-MainActivity";

    public GameSurfaceView gameSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        gameSurfaceView = (GameSurfaceView)findViewById(R.id.gameSurfaceView);
        gameSurfaceView.init(this);

        gameSurfaceView.buttonTest = (Button)findViewById(R.id.buttonTest);
        gameSurfaceView.buttonTest.setOnClickListener(gameSurfaceView);

        gameSurfaceView.buttonUp = (Button)findViewById(R.id.buttonUp);
        gameSurfaceView.buttonUp.setOnClickListener(gameSurfaceView);

        gameSurfaceView.buttonDown = (Button)findViewById(R.id.buttonDown);
        gameSurfaceView.buttonDown.setOnClickListener(gameSurfaceView);

        gameSurfaceView.buttonLeft = (Button)findViewById(R.id.buttonLeft);
        gameSurfaceView.buttonLeft.setOnClickListener(gameSurfaceView);

        gameSurfaceView.buttonRight = (Button)findViewById(R.id.buttonRight);
        gameSurfaceView.buttonRight.setOnClickListener(gameSurfaceView);

        gameSurfaceView.buttonEnter = (Button)findViewById(R.id.buttonEnter);
        gameSurfaceView.buttonEnter.setOnClickListener(gameSurfaceView);

    }
}
