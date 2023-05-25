#pragma once

#include <Canis/External/TMXLoader/TMXLoader.h>

class PlantableGroundComponent;

struct GlobalTiledIDS
{
    unsigned int gid = 1;
    std::string tileSetName = "";
    std::string path = "";
};

class MapBuilder : public Canis::ScriptableEntity
{
private:
    int width = 100;
    int height = 100;

public:
    void OnCreate()
    {
    }

    void OnReady()
    {
        int halfWidth = width / 2;
        int halfHeight = height / 2;
        int border = 20;
        float chance = 0.0f;

        Canis::GLTexture texture = GetAssetManager().Get<Canis::TextureAsset>(
                                                        GetAssetManager().LoadTexture("assets/textures/environment/background_sprite_sheet.png"))
                                       ->GetTexture();
        Canis::GLTexture borderTexture = GetAssetManager().Get<Canis::TextureAsset>(
                                                              GetAssetManager().LoadTexture("assets/textures/environment/border_environment_sprite_sheet.png"))
                                             ->GetTexture();

        Canis::TiledMapAsset *map = GetAssetManager().Get<Canis::TiledMapAsset>(GetAssetManager().LoadTiledMap("assets/tiled/testmap_v2.tmx"));

        TMXLoader *loader = (TMXLoader *)map->GetLoader();

        std::vector<GlobalTiledIDS> globalTiledIDS = {};

        std::vector<std::string> names = loader->getMap("map")->getTilesetNames();

        for (std::string s : names)
        {
            GlobalTiledIDS gtid;
            gtid.gid = loader->getMap("map")->getTileset(s)->getFirstGID();
            gtid.tileSetName = s;
            gtid.path = "assets/"+loader->getMap("map")->getTileset(s)->getSource().erase(0,3);

            globalTiledIDS.push_back(gtid);
        }

        int tileWidth = loader->getMap("map")->getTileWidth();
        int tileHeight = loader->getMap("map")->getTileHeight();

        bool hflip = false;
        bool vflip = false;

        for (unsigned int i = 0; i < loader->getMap("map")->getWidth(); ++i)
        {
            for (unsigned int j = 0; j < loader->getMap("map")->getHeight(); ++j)
            {
                // get the tile at current position
                unsigned int gid = loader->getMap("map")->getLayer("Tile Layer 1")->getTiles()[i][j];

                // only render if it is an actual tile (tileID = 0 means no tile / don't render anything here)
                if (gid > 0)
                {
                    hflip = false;
                    vflip = false;

                    // horizonal flip
                    if ((gid & Canis::BIT::THIRTY_TWO) > 0)
                    {
                        hflip = true;
                        gid = gid ^ Canis::BIT::THIRTY_TWO; // set bit 32 to 0
                    }

                    // vertical flip
                    if ((gid & Canis::BIT::THIRTY_ONE) > 0)
                    {
                        vflip = true;
                        gid = gid ^ Canis::BIT::THIRTY_ONE; // set bit 31 to 0
                    }

                    if ((gid & Canis::BIT::THIRTY) > 0)
                    {
                        gid = gid ^ Canis::BIT::THIRTY; // set bit 30 to 0
                    }

                    if ((gid & Canis::BIT::TWENTY_NIVE) > 0)
                    {
                        gid = gid ^ Canis::BIT::TWENTY_NIVE; // set bit 29 to 0
                    }

                    unsigned int firstIdIndex = 0;

                    for (int g = 0; g < globalTiledIDS.size(); g++)
                    {
                        if (globalTiledIDS[g].gid <= gid)
                        {
                            if (globalTiledIDS[g].gid >= globalTiledIDS[firstIdIndex].gid)
                            {
                                firstIdIndex = g;
                                texture = GetAssetManager().Get<Canis::TextureAsset>(
                                                          GetAssetManager().LoadTexture(globalTiledIDS[g].path))
                                         ->GetTexture();
                            }
                        }
                    }

                    unsigned int id = gid - globalTiledIDS[firstIdIndex].gid + 1;

                    Canis::Entity tile = CreateEntity();
                    auto &tileRect = tile.AddComponent<Canis::RectTransformComponent>();
                    tileRect.position.x = j * 32;// + (loader->getMap("map")->getWidth() * 32);
                    tileRect.position.y = (-i * 32) + ((loader->getMap("map")->getHeight()-1) * 32);
                    tileRect.size.x = 32;
                    tileRect.size.y = 32;
                    tileRect.depth = 2.0f;
                    auto &tileColor = tile.AddComponent<Canis::ColorComponent>();
                    tileColor.color = glm::vec4(1.0f);
                    auto &sprite = tile.AddComponent<Canis::Sprite2DComponent>();

                    sprite.texture = texture;

                    GetSpriteFromTextureAtlas(
                        sprite,
                        0,
                        0,
                        (id - 1) % 4, // x index
                        (id) / 4, // y index
                        16,
                        16,
                        hflip,
                        vflip);
                }
            }
        }
    }

    void OnDestroy()
    {
    }

    void OnUpdate(float _dt)
    {
    }
};

bool DecodeMapBuilder(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "MapBuilder")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<MapBuilder>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

/*
for (int x = -halfWidth; x < halfWidth; x++)
            {
                for (int y = -halfHeight; y < halfHeight; y++)
                {
                    Canis::Entity tile = CreateEntity();
                    auto &tileRect = tile.AddComponent<Canis::RectTransformComponent>();
                    tileRect.position.x = x * 32;
                    tileRect.position.y = y * 32;
                    tileRect.size.x = 32;
                    tileRect.size.y = 32;
                    tileRect.depth = 2.0f;
                    auto &tileColor = tile.AddComponent<Canis::ColorComponent>();
                    tileColor.color = glm::vec4(1.0f);
                    auto &sprite = tile.AddComponent<Canis::Sprite2DComponent>();

                    if (x > ((GetWindow().GetScreenWidth() / 64.0f) - halfWidth) &&
                        x < (halfWidth - (GetWindow().GetScreenWidth() / 64.0f)) &&
                        y < (halfHeight - (GetWindow().GetScreenHeight() / 64.0f)) &&
                        y > ((GetWindow().GetScreenHeight() / 64.0f) - halfHeight))
                    {
                        sprite.texture = texture;

                        if (chance < Canis::RandomFloat(0.0f, 100.0f))
                        {
                            GetSpriteFromTextureAtlas(
                                sprite,
                                0,
                                0,
                                rand() % 3,
                                rand() % 3,
                                16,
                                16,
                                (0 == rand() % 2),
                                (0 == rand() % 2));
                            chance += 0.01f;
                        }
                        else
                        {
                            GetSpriteFromTextureAtlas(
                                sprite,
                                0,
                                0,
                                3,
                                1,
                                16,
                                16,
                                (0 == rand() % 2),
                                (0 == rand() % 2));
                            auto &pgc = tile.AddComponent<PlantableGroundComponent>();
                            auto &col = tile.AddComponent<Canis::CircleColliderComponent>();
                            col.layer = Canis::BIT::SEVEN;
                            col.mask = Canis::BIT::ONE;
                            col.radius = tileRect.size.x / 2.0f;
                            chance = 0.0f;
                        }
                    }
                    else
                    {
                        sprite.texture = borderTexture;
                        GetSpriteFromTextureAtlas(
                            sprite,
                            0,
                            0,
                            3,
                            0,
                            16,
                            16,
                            false,
                            false);
                    }
                }
            }
*/