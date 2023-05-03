#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <alsa/asoundlib.h>
#include <unistd.h>

//#include <SDL/SDL_mixer.h>

GLint LARGURA = 800,
      ALTURA = 400;
      
int cont1 = 0;
int cont2 = 0;

bool pausar = false;
bool pausarBola = false;

char continuar;
      
using namespace std;
      
float ball_x = 402.0f, ball_y = 200.0f; // posição da bola
float ball_dx = 4.0f, ball_dy = 4.0f; // velocidade da bola
float player1_y = 200.0f, player2_y = 200.0f; // posição dos jogadores
const float player_speed = 20.0f; // velocidade dos jogadores
const float player_size = 30.0f; // tamanho dos jogadores
const float ball_size = 15.0f; // tamanho da bola
const float field_size = 400.0f; // tamanho do campo

void initGlut(int *argc, char **argv){
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(LARGURA, ALTURA);
    glutInitWindowPosition(50, 50);
    
    // inicializa o SDL Mixer
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    // carrega o arquivo de som do barulho
    //sound = Mix_LoadWAV("barulho.wav");
    
    glutCreateWindow("ping pong");
}

void inicializacao(GLvoid){
    // Define a cor de fundo da janela de visualização como branca
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
                //esquerda, direita, baixo, cima
    gluOrtho2D(0, LARGURA, 0, ALTURA);
}

void beep(unsigned int freq, unsigned int duration)
{
    // Initialize the PCM device
    snd_pcm_t* pcm;
    snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_set_params(pcm, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 500000);

    // Generate the waveform
    const int sample_rate = 44100;
    const double amplitude = 32767 / 2.0;
    const int num_samples = sample_rate * duration / 1000;
    short samples[num_samples];
    for (int i = 0; i < num_samples; i++) {
        double t = (double)i / (double)sample_rate;
        double freq_hz = freq;
        double sine_wave = amplitude * sin(2.0 * M_PI * freq_hz * t);
        samples[i] = (short)sine_wave;
    }

    // Play the waveform
    snd_pcm_sframes_t frames = snd_pcm_writei(pcm, samples, num_samples);
    if (frames < 0) {
        frames = snd_pcm_recover(pcm, frames, 0);
    }

    // Wait for the playback to finish
    snd_pcm_drain(pcm);
    snd_pcm_close(pcm);
}



void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawCircle(float x, float y, float r) {
    // desenha os jogadores e a bola
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (float i = 0.0f; i <= 360.0f; i += 5.0f) {
        glVertex2f(x + r * cos(i * M_PI / 180.0f), y + r * sin(i * M_PI / 180.0f));
    }
    glEnd();
}

void desenhar(){

//Players e bola

glColor3f(0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(5.0f, player1_y - player_size, player_size, player_size * 2);
    drawRect(795.0f - player_size, player2_y - player_size, player_size, player_size * 2);
    drawCircle(ball_x, ball_y, ball_size);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    
//Linha do meio

    drawRect(400, 0, 5, 400);
    drawRect(-field_size, field_size - 0.01f, field_size * 2, 0.02f);
    
    //placar
    
    std::string s1 = std::to_string(cont1); // converte o inteiro em uma string
char c1 = s1[0];

std::string s2 = std::to_string(cont2); // converte o inteiro em uma string
char c2 = s2[0];

    glRasterPos2f(345.0, 370.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c1);
    glRasterPos2f(450.0, 370.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c2);

}


void desenha(GLvoid){
    glClear(GL_COLOR_BUFFER_BIT);
    desenhar();      
    glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    // verifica as teclas pressionadas pelos jogadores
   if(!pausar){
   
    switch (key) {
        case 'w':
        
        if(pausarBola && ball_x <400){
               
               if(ball_y < 360){
               ball_y += player_speed;
                }
                }
        
            player1_y += player_speed;
            if ((player1_y + player_size > field_size)) {
                player1_y = field_size - player_size;
                
            }
            
            break;
        case 's':
        
        if(pausarBola && ball_x < 400){
            if(ball_y > 40){
               ball_y -= player_speed;
            }
        }
             
            player1_y -= player_speed;
            if (player1_y - player_size < 0) {
                player1_y = 0 + player_size;
            }
            break;
            
        case 13:
           
           pausarBola = false;
            
            break;
            
            }
            }
            
            switch (key) {
            	
            	case ' ':
            
            beep(350,100);
            beep(280, 150);
            pausar = !pausar;
            break;
            
            }
            	
    
    
    glutPostRedisplay();
}

void special(int key, int x, int y) {

if(!pausar){
    switch (key) {
        case GLUT_KEY_UP:
        
         if(pausarBola && ball_x > 400){
               
               if(ball_y < 360){
               ball_y += player_speed;
                }
                }
        
 		player2_y += player_speed;
            	
            	if (player2_y + player_size > field_size) {
                	player2_y = field_size - player_size;
            	}
        break;
        
        case GLUT_KEY_DOWN:
        
         if(pausarBola && ball_x > 400){
            if(ball_y > 40){
               ball_y -= player_speed;
            }
        }
        
 		player2_y -= player_speed;
            	if (player2_y - player_size < 0) {
                player2_y = 0 + player_size;
            	}
            	break;
                
	}       
	}
	glutPostRedisplay();
}

void resetValue1(){

	ball_x = 50;
        ball_y = 202.0f;
        player1_y = 200.0f;
        player2_y = 200.0f;
        ball_dx = 4.0f;

}

void resetValue2(){

	ball_x = 745;
        ball_y = 202.0f;
        player1_y = 200.0f;
        player2_y = 200.0f;
        ball_dx = 4.0f;

}

void resetValue(){

	ball_x = 402.0f;
        ball_y = 200.0f;
        player1_y = 200.0f;
        player2_y = 200.0f;
        ball_dx = 4.0f;

}

void update(int value) {

if(!pausar){

if(!pausarBola){

reiniciar:

myGoto:

//while((cont1 < 7) || (cont2 < 7)){
    // atualiza a posição da bola
    ball_x += ball_dx;
    ball_y += ball_dy;

    // verifica colisão com as paredes superior e inferior
    if (ball_y + ball_size > 400 || ball_y - ball_size < 0) {
        ball_dy = -ball_dy;
    }

    // verifica colisão com os jogadores
    if (ball_x - ball_size <= 0.0f + player_size && ball_y - ball_size < player1_y + player_size && ball_y + ball_size > player1_y - player_size) {
    	beep(250,95);
        ball_dx = -ball_dx;
        ball_dx += 0.5;
    }
    if (ball_x + ball_size >= 800.0f - player_size && ball_y - ball_size < player2_y + player_size && ball_y + ball_size > player2_y - player_size) {
    	beep(250,95);
        ball_dx = -ball_dx;
        ball_dx += 0.5;
    }
    // verifica se a bola saiu do campo
    if (ball_x + ball_size > 800) {
        //cout << "Player 1 ganhou!" << endl;
       // Mix_PlayChannel(-1, sound, 0);
             cont1++;
              beep(400, 100);
             pausarBola = !pausarBola;
            printf("Player 1: %d pontos\n", cont1);
            if (cont1 == 4) {
            
            	beep(300,80);
            	beep(350,90);
            	beep(400, 100);
            	beep(500, 150);
                printf("Parabens, Player 1 venceu!\n\n deseja jogar novamente? (s/n)");
                scanf(" %c", &continuar);
                
                if(continuar == 's'){
                cont1 = 0;
                cont2 = 0;
                resetValue();
                goto reiniciar;
                
                }else{
                exit(0);
                }
            }
            else {
              resetValue1();
              goto myGoto;
            }
            //exit(0);
    }
    if (ball_x - ball_size < 0) {
       // cout << "Player 2 ganhou!" << endl;
      // Mix_PlayChannel(-1, sound, 0);
       	   cont2++;
       	   beep(400, 100);
       	   pausarBola = !pausarBola;
       	   printf("Player 2: %d pontos\n", cont2);
            if (cont2 == 4) {
            	beep(300,80);
            	beep(350,90);
            	beep(400, 100);
            	beep(500, 150);
                printf("Parabens, Player 2 venceu!\n\nDeseja jogar novamente?(s/n)");
                scanf(" %c", &continuar);
                
                if(continuar == 's'){
                cont1 = 0;
                cont2 = 0;
                resetValue();
                goto reiniciar;
                
                }else{
                exit(0);
                }
            }
            else {
              resetValue2();
              goto myGoto;
            }
           //exit(0);
    }

}
}
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
    
}

int main(int argc, char *argv[])
{
    initGlut(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    inicializacao();
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(desenha);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
