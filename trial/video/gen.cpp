#include <stdio.h>
#include <unistd.h>

#include "moyai/moyai.h"


Moyai *g_moyai;


void resize(int w, int h ) {}

void display(void) {
    g_moyai->renderAll();
}

#define NPROP 60

Prop *g_props[NPROP];

void idle(void) {
    static int cnt = 0;
    cnt++;
    static double last_poll_at = now();
    double t = now();
    double dt = t - last_poll_at;

    if( cnt == 200 ) {
        print("done");
        exit(0);
    }

    int mod = sqrt(NPROP) - 1;
    assert(mod>0);
    for(int i=0;i<NPROP;i++){
        Prop *p = g_props[i];

        float dx = -mod/2 + (i%mod);
        float dy = -mod/2 + (i/mod);
        p->loc += Vec2( dx, dy ) * dt * 100;
    }


    g_moyai->pollAll(dt);
    g_moyai->renderAll();
    last_poll_at = t;
}


int main( int argc, char **argv ) {

    glutInit( &argc, argv );
    glutInitWindowSize(320,240);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    //    glutKeyboardFunc(keyboard_down);
    //    glutKeyboardUpFunc(keyboard_up);
    glutIdleFunc(idle);

    glClearColor(0,0,0,1);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true );        

    

    g_moyai = new Moyai();
    
    Texture *t = new Texture();
    t->load( "moyai/assets/base.png" );
    TileDeck *dk = new TileDeck();
    dk->setTexture(t);
    dk->setSize( 16,16, 16,16, 256,256 );

    Viewport *vp = new Viewport();
    vp->setSize( 320, 240 );
    vp->setScale( 320, 240 );

    Layer *l = new Layer();
    l->setViewport(vp);
    g_moyai->insertLayer(l);

    for(int i=0;i<NPROP;i++){
        Prop *p = new Prop();
        p->setDeck(dk);
        p->setScl(16,16);
        p->setLoc(range(-100,100),range(-100,100) );
        p->setIndex(0);

        l->insertProp(p);

        g_props[i] = p;
    }
    
    glutMainLoop();
    
    
}
