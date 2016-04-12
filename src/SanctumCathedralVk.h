//
//  SanctumCathedral.h
//  SanctumVR
//
//  Created by Felix Faire on 15/12/2015.
//
//

#ifndef SanctumCathedral_h
#define SanctumCathedral_h

#include "cinder/vk/vk.h"

using namespace ci;
using namespace ci::app;
using namespace std;



class SanctumCathedral
{
public:
    SanctumCathedral()
    {
    }

    static vk::TextureRef createTexture(char* fileName, const vk::Texture::Format& format = vk::Texture::Format())
    {
        auto surface = Surface::create(loadImage(loadAsset(fileName)));
        auto tex = vk::Texture::create(*surface, format);

        return tex;
    }

    void setupModel()
    {
        vk::Texture::Format format;
        format.mipmap(true);
        //format.setMaxAnisotropy( 32.0f );

#if 0
        auto shaderDef = vk::ShaderDef().texture();
        mDefaultShader = vk::getStockShader(shaderDef);
#else
        vk::ShaderProg::Format shaderFormat = vk::ShaderProg::Format()
#if defined( USE_SPIRV )
            .vertex(loadAsset("shader.vert.spirv"))
            .fragment(loadAsset("shader.frag.spirv"));
#else
            .vertex(loadAsset("shader.vert"))
            .fragment(loadAsset("shader.frag"));
#endif
        mDefaultShader = vk::GlslProg::create(shaderFormat);
        mDefaultShader->uniform("ciBlock1.uAmbient", vec4(1,1,1,1));
#endif
        auto tex = createTexture("textures/ElyCeiling.png", format);
        auto ceiling = createCeilingGeometry();
        mVault = vk::Batch::create(ceiling, mDefaultShader);
        mVault->uniform("uTex0", tex);

        tex = createTexture("textures/ElyCeilingBack.jpg", format);
        mVaultPiping = vk::Batch::create(ceiling, mDefaultShader);
        mVaultPiping->uniform("uTex0", tex);

        tex = createTexture("textures/ArchSides.jpg", format);
        mWindowSides = vk::Batch::create(createWindowSidesGeometry(), mDefaultShader);
        mWindowSides->uniform("uTex0", tex);

        tex = createTexture("textures/ElyEndWindows.jpg", format);
        mMainWindows = vk::Batch::create(createMainWindowGeometry(), mDefaultShader);
        mMainWindows->uniform("uTex0", tex);

        tex = createTexture("textures/ElyFloor.jpg", format);
        mFloor = vk::Batch::create(geom::Plane().size(vec2(27.15f, 60.96f))
                                   .normal(vec3(0.0f, -1.0f, 0.0f))
                                   >> geom::Translate(0.0f, -6.6f, 0.0f),
                                   mDefaultShader);
        mFloor->uniform("uTex0", tex);

        tex = createTexture("textures/ElySideWindowsBack.jpg", format);
        mWindowPanes = vk::Batch::create(createWindowPanesGeometry(), mDefaultShader);
        mWindowPanes->uniform("uTex0", tex);

        tex = createTexture("textures/ElySideWindowsBack2.png", format);
        mWindowMullions = vk::Batch::create(createWindowMullionsGeometry(), mDefaultShader);
        mWindowMullions->uniform("uTex0", tex);
        tex = createTexture("textures/ElySideWindowsCentre2.png", format);
        mCentreMullions = vk::Batch::create(createCentreMullionsGeometry(), mDefaultShader);
        mCentreMullions->uniform("uTex0", tex);
        tex = createTexture("textures/ElySideWindowsCentre.jpg", format);
        mCentrePanes = vk::Batch::create(createCentrePanesGeometry(), mDefaultShader);
        mCentrePanes->uniform("uTex0", tex);

        tex = createTexture("textures/ElyEndColumns.jpg", format);
        mEndColumns = vk::Batch::create(createEndColumGeometry(), mDefaultShader);
        mEndColumns->uniform("uTex0", tex);

        tex = createTexture("textures/ElySideWalls.png", format);
        mWalls = vk::Batch::create(createWallsGeometry(), mDefaultShader);
        mWalls->uniform("uTex0", tex);

        tex = createTexture("textures/LightBeam.png", format);
        mLight = vk::Batch::create(createLightGeometry(), mDefaultShader);
        mLight->uniform("uTex0", tex);
        mLight->uniform("ciBlock1.uAmbient", vec4(1.0f, 1.0f, 1.0f, 0.15f));
    }

    void update()
    {
        {
            vk::ScopedModelMatrix matrix;
            vk::translate(vec3(0.0f, -0.2f, 0.0f));
            updateBatch(mVaultPiping);
        }

        updateBatch(mVault);
        updateBatch(mMainWindows);
        updateBatch(mWindowPanes);
        updateBatch(mWindowMullions);
        updateBatch(mCentrePanes);
        updateBatch(mEndColumns);
        updateBatch(mWindowSides);
        updateBatch(mLight);
        updateBatch(mWalls);
        updateBatch(mCentreMullions);
        updateBatch(mFloor);
    }

    void draw()
    {
        vk::enableAlphaBlending();
        //vk::ScopedColor mainClr{ 1.0f, 1.0f, 1.0f, 1.0f };
        {
            drawMainWindows();
            drawEndColumns();
            drawCeiling();
            drawSideWindows();
            drawFloor();
            drawWindowSides();
            drawWalls();
        }

        //vk::ScopedColor lightClr{ 1.0f, 1.0f, 1.0f, 0.15f };
        {
            drawLightBeams();
        }
    }

private:

    // GEOMETRY FUNCTIONS ////////////////////////////////////////////////////

    geom::SourceMods createCeilingGeometry()
    {
        ObjLoader vaultGeom(loadAsset("obj/ArchElement.obj"));
        geom::SourceMods arches;

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                arches &= vaultGeom
                          >> geom::Rotate((j > 0 ? -1.0f : 1.0f) * M_PI * 0.5f, vec3(0.0f, 1.0f, 0.0f))
                          >> geom::Translate(0.0f, 31.71f, 24.38f - (float)i * 12.19f);
            }
        }
        return arches;
    }

    geom::SourceMods createMainWindowGeometry()
    {
        geom::SourceMods mainWindows;

        for (int i = 0; i < 2; i++)
            mainWindows &= geom::Plane().size(vec2(27.15f, 37.31f)).normal(vec3(0.0f, 0.0f, -1.0f))
                           >> geom::Translate(0.0f, 12.0f, 30.48f)
                           >> geom::Rotate(M_PI * (float)i, vec3(0.0f, 1.0f, 0.0f));

        return mainWindows;
    }

    geom::SourceMods createWindowPanesGeometry()
    {
        geom::SourceMods windowPanes;

        for (int i = -2; i < 3; i++)
            for (int j = -1; j < 2; j++)
                if (i != 0 && j != 0)
                    windowPanes &= geom::Plane().size(vec2(12.19f, 37.31f)).normal(vec3(1.0f, 0.0f, 0.0f))
                                   >> geom::Rotate((float)M_PI * 0.5, vec3(-1.0f, 0.0f, 0.0f))
                                   >> geom::Translate((float)j * 14.99f, 12.0f, (float)i * 12.19f);

        return windowPanes;
    }

    geom::SourceMods createWindowMullionsGeometry()
    {
        geom::SourceMods windowMullions;

        for (int i = -2; i < 3; i++)
            for (int j = -1; j < 2; j++)
                if (i != 0 && j != 0)
                    windowMullions &= geom::Plane().size(vec2(12.19f, 37.31f)).normal(vec3(1.0f, 0.0f, 0.0f))
                                      >> geom::Rotate((float)M_PI * 0.5, vec3(-1.0f, 0.0f, 0.0f))
                                      >> geom::Translate((float)j * 14.77f, 12.0f, (float)i * 12.19f);

        return windowMullions;
    }

    geom::SourceMods createCentrePanesGeometry()
    {
        geom::SourceMods centrePanes;

        for (int i = -1; i < 2; i++)
            if (i != 0)
                centrePanes &= geom::Plane().size(vec2(12.19f, 37.31f)).normal(vec3(1.0f, 0.0f, 0.0f))
                               >> geom::Rotate((float)M_PI * 0.5, vec3(-1.0f, 0.0f, 0.0f))
                               >> geom::Translate((float)i * 14.99, 12.0f, -0.15f);

        return centrePanes;
    }

    geom::SourceMods createCentreMullionsGeometry()
    {
        geom::SourceMods centreMullions;

        for (int i = -1; i < 2; i++)
            if (i != 0)
                centreMullions &= geom::Plane().size(vec2(12.19f, 37.31f)).normal(vec3(1.0f, 0.0f, 0.0f))
                                  >> geom::Rotate((float)M_PI * 0.5, vec3(-1.0f, 0.0f, 0.0f))
                                  >> geom::Translate((float)i * 14.77, 12.0f, -0.15f);

        return centreMullions;
    }

    geom::SourceMods createEndColumGeometry()
    {
        geom::SourceMods endColums;

        for (int i = -1; i < 2; i++)
            for (int j = -1; j < 2; j++)
                if (j != 0 && i != 0)
                    endColums &= geom::Plane().size(vec2(4.0f, 30.02f)).normal(vec3(0.0f, 0.0f, -1.0f))
                                 >> geom::Translate((float)i * 11.6f, 8.46f, (float)j * 30.0f);

        return endColums;
    }

    geom::SourceMods createWindowSidesGeometry()
    {
        geom::SourceMods windowSides;
        ObjLoader side(loadAsset("obj/WindowSide.obj"));

        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 2; j++)
                windowSides &= side
                               >> geom::Rotate((j > 0 ? 1.0f : -1.0f) * (float)M_PI * 0.5f, vec3(0.0f, 1.0f, 0.0f))
                               >> geom::Translate(j > 0 ? -13.57f : 13.57f, 26.91f, 24.38 - (float)i * 12.19f);

        return windowSides;
    }

    geom::SourceMods createWallsGeometry()
    {
        geom::SourceMods walls;

        for (int i = -2; i < 3; i++)
            for (int j = -1; j < 2; j++)
                if (j != 0)
                    walls &= geom::WirePlane().size(vec2(12.19f, 37.31f)).normal(vec3(1.0f, 0.0f, 0.0f))
                             >> geom::Rotate((float)M_PI * 0.5, vec3(-1.0f, 0.0f, 0.0f))
                             >> geom::Translate((float)j * 13.57f, 12.0f, (float)i * 12.19f);

        return walls;
    }

    geom::SourceMods createLightGeometry()
    {
        geom::SourceMods lightGeometry;

        TriMesh lightMesh;

        lightMesh.appendTexCoord(vec2(1.0f, 1.0f));
        lightMesh.appendPosition(vec3(-13.5, 25.5, -6.9));

        lightMesh.appendTexCoord(vec2(0.0f, 1.0f));
        lightMesh.appendPosition(vec3(-11.7, 2.2, -5.8));

        lightMesh.appendTexCoord(vec2(1.0f, 0.0f));
        lightMesh.appendPosition(vec3(9.7, -6.0, 6.7));

        lightMesh.appendTexCoord(vec2(0.0f, 0.0f));
        lightMesh.appendPosition(vec3(-7.6, -6.0, -3.5));

        for (int i = 0; i < 4; i++)
        {
            lightGeometry &= lightMesh
                             >> geom::Translate(-1.0f, 0.0f, 19.5f - 13.22f * (float)i);
        }

        return lightGeometry;
    }

    // DRAWING FUNCTIONS ////////////////////////////////////////////////////

    void updateBatch(vk::BatchRef batch)
    {
        batch->setDefaultUniformVars(vk::context());
        vk::context()->addPendingUniformVars(batch);
    }

    void drawCeiling()
    {
        mVault->draw();

        mVaultPiping->draw();
    }

    void drawMainWindows()
    {
        mMainWindows->draw();
    }

    void drawSideWindows()
    {
        mWindowPanes->draw();

        mWindowMullions->draw();

        mCentrePanes->draw();

        mCentreMullions->draw();
    }

    void drawFloor()
    {
        mFloor->draw();
    }

    void drawEndColumns()
    {
        vk::disableDepthWrite();
        mEndColumns->draw();
        vk::enableDepthWrite();
    }

    void drawWindowSides()
    {
        mWindowSides->draw();
    }

    void drawWalls()
    {
        mWalls->draw();
    }

    void drawLightBeams()
    {
        mLight->draw();
    }

    vk::BatchRef        mVault;
    vk::BatchRef        mVaultPiping;

    // TODO: describe the draw
    vk::BatchRef        mMainWindows;
    vk::BatchRef        mFloor;
    vk::BatchRef        mWindowPanes;
    vk::BatchRef        mWindowMullions;
    vk::BatchRef        mCentrePanes;
    vk::BatchRef        mCentreMullions;
    vk::BatchRef        mEndColumns;
    vk::BatchRef        mWindowSides;
    vk::BatchRef        mWalls;
    vk::BatchRef        mLight;

    vk::GlslProgRef     mDefaultShader;

};

#endif /* SanctumCathedral_h */
