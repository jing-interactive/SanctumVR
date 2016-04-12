//
//  SanctumCathedral.h
//  SanctumVR
//
//  Created by Felix Faire on 15/12/2015.
//
//

#ifndef SanctumCathedral_h
#define SanctumCathedral_h

using namespace ci;
using namespace ci::app;
using namespace std;



class SanctumCathedral
{
public:
    SanctumCathedral()
    {
    }
    
    void setupModel()
    {
        gl::Texture::Format format;
        format.mipmap(true);
        format.setMaxAnisotropy( 32.0f );
        
        gl::ShaderDef glsl  = gl::ShaderDef().texture().color();
        
        mVaultPipingTex     = gl::Texture::create( loadImage( loadAsset( "textures/ElyCeiling.png" ) ), format );
        mVaultTex           = gl::Texture::create( loadImage( loadAsset( "textures/ElyCeilingBack.jpg" ) ), format );
        mVault              = gl::Batch::create( createCeilingGeometry(), gl::getStockShader( glsl ) );
        
        mWindowSidesTex     = gl::Texture::create( loadImage( loadAsset( "textures/ArchSides.jpg" ) ), format );
        mWindowSides        = gl::Batch::create( createWindowSidesGeometry(), gl::getStockShader( glsl ) );

        mMainWindowsTex     = gl::Texture::create( loadImage( loadAsset( "textures/ElyEndWindows.jpg" ) ), format );
        mMainWindows        = gl::Batch::create( createMainWindowGeometry(),
                                                 gl::getStockShader( glsl ) );
        
        mFloorTex           = gl::Texture::create( loadImage( loadAsset( "textures/ElyFloor.jpg" ) ), format );
        mFloor              = gl::Batch::create( geom::Plane().size( vec2( 27.15f, 60.96f ) )
                                                              .normal( vec3( 0.0f, -1.0f, 0.0f ) )
                                                              >> geom::Translate( 0.0f, -6.6f, 0.0f ),
                                                 gl::getStockShader( glsl ) );
       
        mWindowPanesTex     = gl::Texture::create( loadImage( loadAsset( "textures/ElySideWindowsBack.jpg" ) ), format );
        mWindowPanes        = gl::Batch::create( createWindowPanesGeometry(), gl::getStockShader( glsl ) );
        mWindowMullionsTex  = gl::Texture::create( loadImage( loadAsset( "textures/ElySideWindowsBack2.png" ) ), format );
        mWindowMullions     = gl::Batch::create( createWindowMullionsGeometry(), gl::getStockShader( glsl ) );
        mCentreMullionsTex  = gl::Texture::create( loadImage( loadAsset( "textures/ElySideWindowsCentre2.png" ) ), format );
        mCentreMullions     = gl::Batch::create( createCentreMullionsGeometry(), gl::getStockShader( glsl ) );
        mCentrePanesTex     = gl::Texture::create( loadImage( loadAsset( "textures/ElySideWindowsCentre.jpg" ) ), format );
        mCentrePanes        = gl::Batch::create( createCentrePanesGeometry(), gl::getStockShader( glsl ) );
        
        mEndColumnsTex      = gl::Texture::create( loadImage( loadAsset( "textures/ElyEndColumns.jpg" ) ), format );
        mEndColumns         = gl::Batch::create( createEndColumGeometry(), gl::getStockShader( glsl ) );
        
        mWallsTex           = gl::Texture::create( loadImage( loadAsset( "textures/ElySideWalls.png" ) ), format );
        mWalls              = gl::Batch::create( createWallsGeometry(), gl::getStockShader( glsl ) );
        
        mLightTex           = gl::Texture::create( loadImage( loadAsset( "textures/LightBeam.png" ) ), format );
        mLight              = gl::Batch::create( createLightGeometry(), gl::getStockShader( glsl ) );
    
    }
    
    void draw()
    {
        gl::enableAlphaBlending();
        gl::color( 1.0f, 1.0f, 1.0f, 1.0f );
        drawMainWindows();
        drawEndColumns();
        drawCeiling();
        drawSideWindows();
        drawFloor();
        drawWindowSides();
        drawWalls();
        gl::color( 1.0f, 1.0f, 1.0f, 0.15f );
        drawLightBeams();
    }
    
    
private:
    
    // GEOMETRY FUNCTIONS ////////////////////////////////////////////////////
    
    geom::SourceMods createCeilingGeometry()
    {
        ObjLoader vaultGeom( loadAsset( "obj/ArchElement.obj" ) );
        geom::SourceMods arches;
        
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                arches &= vaultGeom
                       >> geom::Rotate(  (j > 0 ? -1.0f : 1.0f) * M_PI * 0.5f, vec3( 0.0f, 1.0f, 0.0f ) )
                       >> geom::Translate( 0.0f, 31.71f, 24.38f - (float)i * 12.19f );
            }
        }
        return arches;
    }
    
    geom::SourceMods createMainWindowGeometry()
    {
        geom::SourceMods mainWindows;
        
        for (int i = 0; i < 2; i++)
            mainWindows &= geom::Plane().size( vec2( 27.15f, 37.31f ) ).normal( vec3( 0.0f, 0.0f, -1.0f ) )
                        >> geom::Translate( 0.0f, 12.0f, 30.48f )
                        >> geom::Rotate( M_PI * (float)i, vec3( 0.0f, 1.0f, 0.0f ) );
        
        return mainWindows;
    }
    
    geom::SourceMods createWindowPanesGeometry()
    {
        geom::SourceMods windowPanes;
    
        for (int i = -2; i < 3; i++)
            for (int j = -1; j < 2; j++)
                if (i != 0 && j != 0)
                    windowPanes &= geom::Plane().size( vec2( 12.19f, 37.31f ) ).normal( vec3( 1.0f, 0.0f, 0.0f ) )
                                >> geom::Rotate( (float)M_PI * 0.5, vec3( -1.0f, 0.0f, 0.0f ) )
                                >> geom::Translate( (float)j * 14.99f, 12.0f, (float)i * 12.19f );

        return windowPanes;
    }
    
    geom::SourceMods createWindowMullionsGeometry()
    {
        geom::SourceMods windowMullions;
    
        for (int i = -2; i < 3; i++)
            for (int j = -1; j < 2; j++)
                if (i != 0 && j != 0)
                    windowMullions &= geom::Plane().size( vec2( 12.19f, 37.31f ) ).normal( vec3( 1.0f, 0.0f, 0.0f ) )
                                   >> geom::Rotate( (float)M_PI * 0.5, vec3( -1.0f, 0.0f, 0.0f ) )
                                   >> geom::Translate( (float)j * 14.77f, 12.0f, (float)i * 12.19f );

        return windowMullions;
    }
    
    geom::SourceMods createCentrePanesGeometry()
    {
        geom::SourceMods centrePanes;
    
        for (int i = -1 ; i < 2; i++)
            if (i != 0)
                centrePanes &= geom::Plane().size( vec2( 12.19f, 37.31f ) ).normal( vec3( 1.0f, 0.0f, 0.0f ) )
                            >> geom::Rotate( (float)M_PI * 0.5, vec3( -1.0f, 0.0f, 0.0f ) )
                            >> geom::Translate( (float)i * 14.99, 12.0f, -0.15f );
        
        return centrePanes;
    }
    
    geom::SourceMods createCentreMullionsGeometry()
    {
        geom::SourceMods centreMullions;
    
        for (int i = -1 ; i < 2; i++)
            if (i != 0)
                centreMullions &= geom::Plane().size( vec2( 12.19f, 37.31f ) ).normal( vec3( 1.0f, 0.0f, 0.0f ) )
                               >> geom::Rotate( (float)M_PI * 0.5, vec3( -1.0f, 0.0f, 0.0f ) )
                               >> geom::Translate( (float)i * 14.77, 12.0f, -0.15f );
        
        return centreMullions;
    }
    
    geom::SourceMods createEndColumGeometry()
    {
        geom::SourceMods endColums;
        
        for (int i = -1; i < 2; i++)
            for (int j = -1; j < 2; j++)
                if (j != 0 && i != 0)
                    endColums &= geom::Plane().size( vec2( 4.0f, 30.02f ) ).normal( vec3( 0.0f, 0.0f, -1.0f ) )
                              >> geom::Translate( (float)i * 11.6f, 8.46f, (float)j * 30.0f );
        
        return endColums;
    }

    geom::SourceMods createWindowSidesGeometry()
    {
        geom::SourceMods windowSides;
        ObjLoader side( loadAsset( "obj/WindowSide.obj" ) );
        
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 2; j++)
                    windowSides &= side
                                >> geom::Rotate( (j > 0 ? 1.0f : -1.0f) * (float)M_PI * 0.5f, vec3( 0.0f, 1.0f, 0.0f ) )
                                >> geom::Translate( j > 0 ? -13.57f : 13.57f, 26.91f, 24.38 - (float)i * 12.19f );
        
        return windowSides;
    }
    
    geom::SourceMods createWallsGeometry()
    {
        geom::SourceMods walls;
        
        for (int i = -2; i < 3; i++)
            for (int j = -1; j < 2; j++)
                if (j != 0)
                    walls &= geom::Plane().size( vec2( 12.19f, 37.31f ) ).normal( vec3( 1.0f, 0.0f, 0.0f ) )
                          >> geom::Rotate( (float)M_PI * 0.5, vec3( -1.0f, 0.0f, 0.0f ) )
                          >> geom::Translate( (float)j * 13.57f, 12.0f, (float)i * 12.19f );
        
        return walls;
    }
    
    geom::SourceMods createLightGeometry()
    {
        geom::SourceMods lightGeometry;
        
        gl::VertBatchRef lightMesh = gl::VertBatch::create( GL_TRIANGLES );
        lightMesh->begin( GL_TRIANGLE_STRIP );
        lightMesh->texCoord( 1.0f, 1.0f );
        lightMesh->vertex( vec3( -13.5, 25.5, -6.9 ) );
        
        lightMesh->texCoord( 0.0f, 1.0f );
        lightMesh->vertex( vec3( -11.7, 2.2, -5.8 ) );
        
        lightMesh->texCoord( 1.0f, 0.0f );
        lightMesh->vertex( vec3( 9.7, -6.0, 6.7 ) );
        
        lightMesh->texCoord( 0.0f, 0.0f );
        lightMesh->vertex( vec3( -7.6, -6.0, -3.5 ) );
        lightMesh->end();
        
        for (int i = 0; i < 4; i++)
        {
            lightGeometry &= *lightMesh.get()
                          >> geom::Translate( -1.0f, 0.0f, 19.5f - 13.22f * (float)i );
        }
        
        return lightGeometry;
    }

    // DRAWING FUNCTIONS ////////////////////////////////////////////////////
    
    void drawCeiling()
    {
        mVaultTex->bind();
        mVault->draw();
        
        mVaultPipingTex->bind();
        gl::pushMatrices();
        gl::translate( vec3( 0.0f, -0.2f, 0.0f ) );
        mVault->draw();
        gl::popMatrices();
    }
    
    void drawMainWindows()
    {
        mMainWindowsTex->bind();
        mMainWindows->draw();
    }
    
    void drawSideWindows()
    {
        mWindowPanesTex->bind();
        mWindowPanes->draw();
        
        mWindowMullionsTex->bind();
        mWindowMullions->draw();
        
        mCentrePanesTex->bind();
        mCentrePanes->draw();
        
        mCentreMullionsTex->bind();
        mCentreMullions->draw();
    }
    
    void drawFloor()
    {
        mFloorTex->bind();
        mFloor->draw();
    }
    
    void drawEndColumns()
    {
        gl::disableDepthWrite();
        mEndColumnsTex->bind();
        mEndColumns->draw();
        gl::enableDepthWrite();
    }
    
    void drawWindowSides()
    {
        mWindowSidesTex->bind();
        mWindowSides->draw();
    }
    
    void drawWalls()
    {
        mWallsTex->bind();
        mWalls->draw();
    }
    
    void drawLightBeams()
    {
        mLightTex->bind();
        mLight->draw();
    }
    
    gl::BatchRef        mVault;
    gl::TextureRef      mVaultTex;
    gl::TextureRef      mVaultPipingTex;
    
    gl::BatchRef        mMainWindows;
    gl::TextureRef      mMainWindowsTex;
    
    gl::BatchRef        mFloor;
    gl::TextureRef      mFloorTex;
    
    gl::BatchRef        mWindowPanes;
    gl::TextureRef      mWindowPanesTex;

    gl::BatchRef        mWindowMullions;
    gl::TextureRef      mWindowMullionsTex;
    
    gl::BatchRef        mCentrePanes;
    gl::TextureRef      mCentrePanesTex;
    
    gl::BatchRef        mCentreMullions;
    gl::TextureRef      mCentreMullionsTex;
    
    gl::BatchRef        mEndColumns;
    gl::TextureRef      mEndColumnsTex;
    
    gl::BatchRef        mWindowSides;
    gl::TextureRef      mWindowSidesTex;
    
    gl::BatchRef        mWalls;
    gl::TextureRef      mWallsTex;
    
    gl::BatchRef        mLight;
    gl::TextureRef      mLightTex;
    
};

#endif /* SanctumCathedral_h */
