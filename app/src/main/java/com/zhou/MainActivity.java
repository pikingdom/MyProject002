package com.zhou;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RelativeLayout;

import com.example.administrator.hellojni_ndkbuild.R;
import com.nd.hilauncherdev.launcher.LauncherAnimationHelp;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class MainActivity extends Activity  implements OnClickListener{
	
	Button button;
	Button button2;
	Button button3;
	Button button4;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main1);
		button=(Button)findViewById(R.id.button1);
		button2=(Button)findViewById(R.id.button2);
		button3=(Button)findViewById(R.id.button3);
		button4=(Button)findViewById(R.id.button4);
		
		button.setOnClickListener(this);
		button2.setOnClickListener(this);
		button3.setOnClickListener(this);
		button4.setOnClickListener(this);
		button4.setBackgroundColor(0xff00ff00);
		PackageManager manager=getPackageManager();
	
		
		
	}
	
	 static
	  {
	    
	    try
	    {
	      System.loadLibrary("image");
	      Log.e("zhou", "加载成功");
	    }
	    catch (Exception e)
	    {
	      Log.e("zhou", "加载失败");
	      e.printStackTrace();
	    }
	  }
	@Override
	public void onClick(View v) {
		 blurBackground();
	}

	/**
	 * 糊模桌面的背景
	 * */
	void blurBackground()
	{
		Bitmap bitmap=getDrawCache();
		LauncherAnimationHelp.native_blur(bitmap,5,2,0,200);
		saveBitmap(bitmap,"ss");
		
	}
	
	public Bitmap  getDrawCache()
	{	
		View view=getWindow().getDecorView();
		
		Bitmap bitmap_BG=BitmapFactory.decodeResource(getResources(), R.drawable.girl1);
		
		int w=view.getWidth();
		int h=view.getHeight();
		Bitmap bitmap=Bitmap.createBitmap(w,h,Bitmap.Config.ARGB_8888);
		Canvas canvas=new Canvas();
		canvas.setBitmap(bitmap);
		canvas.drawBitmap(bitmap_BG, 0, 0, new Paint());
		//canvas.scale(0.25f,0.25f);
		//view.draw(canvas);
		saveBitmap(bitmap, "tt");
		return bitmap;	
	}
	//static int   a=0;
    /** 保存方法 */
          public  void saveBitmap(Bitmap bm,String picName) {

			  RelativeLayout rl = (RelativeLayout)findViewById(R.id.rl);
			  if(bm != null){
				  rl.setBackground(new BitmapDrawable(bm));
			  }
          //Log.e("zhou", "保存图片");
          File f = new File("/sdcard/360/" , picName+".jpg");

          try {
			  if (f.exists()) {
				  f.delete();
			  } else {
				  f.createNewFile();
			  }
                FileOutputStream out = new FileOutputStream(f);
               bm.compress(Bitmap.CompressFormat.PNG, 90, out);
               out.flush();
               out.close();
               
         } catch (FileNotFoundException e) {
                Log.e("zhou", "文件不存在" );
               e.printStackTrace();
         } catch (IOException e) {
                Log.e("zhou", "写数据IO出错" );
               e.printStackTrace();
         }
   }


}
