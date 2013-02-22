#include <stdio.h>
#include <unistd.h>

#include "moyai/moyai.h"
#include "lz4/lz4.h"


class PlayableImage : public Image {
public:
    unsigned int palette[128];
    int n_palette;    
    PlayableImage() : Image(), n_palette(0) { }


    int searchPalette(unsigned int c) {
        int i;
        for(i=0;i<n_palette;i++) {
            if( palette[i] == c ) {
                return i;
            }
        }
        // 見つからない場合は、割り当てる
        if( i < elementof(palette) - 1 ) {
            int write_ind = n_palette;
            palette[write_ind] = c;
            n_palette ++;
            //            print("reg color[%d]: %d,%d,%d",i, r,g,b);
            return write_ind;
        }
        return -1; // 満杯
    }
    void writePIF( const char *path ) {
        unsigned char *outbuf = (unsigned char *) malloc( height * width * 2);
        
        // Imageのbufferは h*w*4, rgba

        int out_ind=0;
        unsigned int cur_color = 0xff000000; // 下の24ビットだけを使っているので、これで絶対に一致しない

        int contnum=0;
        
        for(int y=0;y<height;y++){
            for(int x=0;x<width;x++){
                int pixel_ind = (x + y * width)*4;
                unsigned int c = (buffer[pixel_ind]<<16) | (buffer[pixel_ind+1]<<8) | buffer[pixel_ind+2]; // RGB. Aは捨てる
                //                print("i:%d CUR:%08x", pixel_ind/4, cur_color );
                if( c == cur_color ) {
                    contnum++;
                    if( contnum == 255 ) {
                        print("M set outbuf[%d]=%d", out_ind, contnum );
                        outbuf[out_ind++] = contnum;
                        contnum = 0;
                        cur_color = 0xff000000;
                    }
                } else {
                    if( cur_color != 0xff000000 ) {
                        assert( out_ind % 2 == 1 );
                        assert( contnum >= 1 && contnum <= 255 );
                        print("C set outbuf[%d]=%d", out_ind, contnum );
                        outbuf[ out_ind++ ] = contnum;
                    } 
                    int ind = searchPalette(c);
                    assert( ind >= 0 && ind < 128 ); // とりあえず止めとく。ほんとは、近い色を検索したりすべき。
                    unsigned char pal_ind = (unsigned char) ind;
                    assertmsg( out_ind % 2 == 0, "odd out_ind:%d", out_ind  );
                    print("N set outbuf[%d]=%d", out_ind, ind );
                    outbuf[ out_ind++ ] = ind;
                    cur_color = c;
                    contnum = 1;
                    //                    prt("i:%d ci:%d(%08x) ", pixel_ind/4, pal_ind, c );
                }
            }
        }
        if( contnum > 0 ) {
            print("L set outbuf[%d]=%d", out_ind, contnum );
            outbuf[out_ind++] = contnum;
        }

        assertmsg( out_ind % 2 == 0, "invalid out len:%d", out_ind );
        
        // verify: lenを合計してみる
        int totlen = 0;
        for(int i=0;i<out_ind/2;i++){
            unsigned char l = outbuf[i*2+1];
            totlen += l;
        }
        assert( totlen == height * width );


        // それを圧縮
        char *compout = (char*)malloc( 64 * 1024 );
        int osz = LZ4_compress( (const char*)outbuf, compout, out_ind );
        print("final size:%d", osz );
        free(compout);
        free(outbuf);
    }
};


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
        if( dx==0 && dy==0 ) dx = 1;
        p->loc += Vec2( dx, dy ) * 0.016 * 200;
    }


    g_moyai->pollAll(0.016);
    g_moyai->renderAll();

    // capture..
    PlayableImage *img = new PlayableImage();
    img->setSize(320,240);
    g_moyai->capture( img );

    char fn[100];
    snprintf(fn,sizeof(fn), "_capt_%03d.png", cnt );
    img->writePNG(fn);
    snprintf(fn,sizeof(fn), "_capt_%03d.pif", cnt );
    img->writePIF(fn);
    delete img;

    sleep(1);
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
