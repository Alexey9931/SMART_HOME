package com.example.smarthomeapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

public class EmailPasswordActivity extends AppCompatActivity implements View.OnClickListener{

    private FirebaseAuth mAuth;
    private FirebaseAuth.AuthStateListener mAuthListener;

    private  EditText ETemail;
    private EditText ETpassword;
    public static String email;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_email_password);

        mAuth = FirebaseAuth.getInstance();
        mAuthListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = firebaseAuth.getCurrentUser();
                if (user != null) {
                    Intent intent = new Intent(EmailPasswordActivity.this,MainActivity.class);
                    startActivity(intent);
                    // User is signed in
                } else {
                    // User is signed out

                }

            }
        };

        ETemail = (EditText) findViewById(R.id.et_email);
        ETpassword = (EditText) findViewById(R.id.et_password);

        findViewById(R.id.btn_sign_in).setOnClickListener(this);
        findViewById(R.id.btn_registration).setOnClickListener(this);

        FirebaseUser user = mAuth.getCurrentUser();
        if(user != null) {
            Intent intent = new Intent(EmailPasswordActivity.this, MainActivity.class);
            ETemail.setText(mAuth.getCurrentUser().getEmail());
            email = ETemail.getText().toString();
            //startActivity(intent);
        }

    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_sign_in)
        {
            if((ETemail.getText().toString().isEmpty()==false)&&(ETpassword.getText().toString().isEmpty()==false))
            {
                signing(ETemail.getText().toString(),ETpassword.getText().toString());
            }
            else
            {
                Toast.makeText(EmailPasswordActivity.this, "Авторизация провалена! Проверьте правильность введенных данных и повторите попытку!", Toast.LENGTH_SHORT).show();
            }
        }else if (view.getId() == R.id.btn_registration)
        {
            if((ETemail.getText().toString().isEmpty()==false)&&(ETpassword.getText().toString().isEmpty()==false))
            {
                registration(ETemail.getText().toString(),ETpassword.getText().toString());
            }
            else
            {
                Toast.makeText(EmailPasswordActivity.this, "Регистрация провалена! Проверьте правильность введенных данных и повторите попытку!", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void signing (String email, String password)
    {
        mAuth.signInWithEmailAndPassword(email,password).addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if(task.isSuccessful())
                {
                    Toast.makeText(EmailPasswordActivity.this, "Авторизация успешна!", Toast.LENGTH_SHORT).show();
                    FirebaseUser user = mAuth.getCurrentUser();
                    if(user != null) {
                        Intent intent = new Intent(EmailPasswordActivity.this, MainActivity.class);
                        startActivity(intent);
                    }

                }else
                    Toast.makeText(EmailPasswordActivity.this, "Авторизация провалена! Проверьте правильность введенных данных и повторите попытку!", Toast.LENGTH_SHORT).show();

            }
        });
    }
    public void registration(String email, String password){
        mAuth.createUserWithEmailAndPassword(email,password).addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if(task.isSuccessful())
                {
                    Toast.makeText(EmailPasswordActivity.this, "Регистрация успешна!", Toast.LENGTH_SHORT).show();
                }else
                    Toast.makeText(EmailPasswordActivity.this, "Регистрация провалена!", Toast.LENGTH_SHORT).show();

            }
        });
    }
}