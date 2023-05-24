#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

#include "App.hpp"

int main(int argc, char* argv[])
{
    App app;

    app.Run();

    return 0;
}

/*#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "TMXLoader/TMXLoader.h"

struct GlobalTiledIDS {
    unsigned int gid = 1;
    std::string tileSetName = "";
};

std::vector<GlobalTiledIDS> globalTiledIDS = {};

namespace Canis
{
    enum BIT
    {
        ZERO          = 0x0000000u,
        ONE           = 0x0000001u,
        TWO           = 0x0000002u,
        THREE         = 0x0000004u,
        FOUR          = 0x0000008u,
        FIVE          = 0x0000010u,
        SIX           = 0x0000020u,
        SEVEN         = 0x0000040u,
        EIGHT         = 0x0000080u,
        NINE          = 0x0000100u,
        TEN           = 0x0000200u,
        ELEVEN        = 0x0000400u,
        TWELVE        = 0x0000800u,
        THIRTEEN      = 0x0001000u,
        FOURTEEN      = 0x0002000u,
        FIFTEEN       = 0x0004000u,
        SIXTEEN       = 0x0008000u,
        SEVENTEEN     = 0x0010000u,
        EIGHTTEEN     = 0x0020000u,
        NINETEEN      = 0x0040000u,
        TWENTY        = 0x0080000u,
        TWENTY_ONE    = 0x0100000u,
        TWENTY_TWO    = 0x0200000u,
        TWENTY_THREE  = 0x0400000u,
        TWENTY_FOUR   = 0x0800000u,
        TWENTY_FIVE   = 0x1000000u,
        TWENTY_SIX    = 0x2000000u,
        TWENTY_SEVEN  = 0x4000000u,
        TWENTY_EIGHT  = 0x8000000u,
        TWENTY_NIVE   = 0x10000000u,
        THIRTY        = 0x20000000u,
        THIRTY_ONE    = 0x40000000u,
        THIRTY_TWO    = 0x80000000u
    };
} // end of Canis namespace

bool update(SDL_Event &events)
{
    while (SDL_PollEvent(&events))
    {
        switch (events.type)
        {
        case SDL_WINDOWEVENT:
            switch (events.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                return false;
                break;
            }
            break;
        case SDL_QUIT:
            return false;
            break;
        }
    }
    return true;
}

void render(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Texture *texture1, TMXLoader *loader)
{
    SDL_SetRenderDrawColor(renderer, 10, 255, 255, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);

    char tileID = 0;

    int tileWidth = loader->getMap("testmap")->getTileWidth();
    int tileHeight = loader->getMap("testmap")->getTileHeight();

    for (unsigned int i = 0; i < loader->getMap("testmap")->getWidth(); ++i)
    {
        for (unsigned int j = 0; j < loader->getMap("testmap")->getHeight(); ++j)
        {
            // get the tile at current position
            unsigned int gid = loader->getMap("testmap")->getLayer("Tile Layer 1")->getTiles()[i][j];

            // only render if it is an actual tile (tileID = 0 means no tile / don't render anything here)
            if (gid > 0)
            {
                // horizonal flip
                if ((gid & Canis::BIT::THIRTY_TWO) > 0)
                {
                    gid = gid ^ Canis::BIT::THIRTY_TWO; // set bit 32 to 0
                }

                // vertical flip
                if ((gid & Canis::BIT::THIRTY_ONE) > 0)
                {
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

                for(int i = 0; i < globalTiledIDS.size(); i++)
                {
                    if (globalTiledIDS[i].gid <= gid)
                        if (globalTiledIDS[i].gid >= globalTiledIDS[firstIdIndex].gid)
                            firstIdIndex = i;
                }

                unsigned int id = gid - globalTiledIDS[firstIdIndex].gid + 1;
                SDL_Rect srcrect;             

                if (globalTiledIDS[firstIdIndex].tileSetName == "defaulttileset")
                    srcrect = {(int)((id - 1) % 3) * tileWidth, (int)((id - 1) / 3) * tileHeight, tileWidth, tileHeight};
                else
                    srcrect = {(int)((id - 1) % 4) * 16, (int)((id - 1) / 4) * 16, 16, 16}; // srcrect = {(int)((id - 1) % 4) * 16, ((int)((id - 1) % 2) * 16) - 16, 16, 16};
                
                SDL_Rect dstrect = {(int)j * 25, (int)i * 25, 25, 25};

                if (globalTiledIDS[firstIdIndex].tileSetName == "defaulttileset")
                    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
                else
                    SDL_RenderCopy(renderer, texture1, &srcrect, &dstrect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

// five min arm
// one min blash

int main(int argc, char *argv[])
{
    unsigned int test = 1073741830;
    unsigned int output = (test & Canis::BIT::THIRTY_ONE);

    if (output > 0)
    {
        std::cout << test << std::endl;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init: " << SDL_GetError() << std::endl;
    }

    Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

    SDL_Window *window = SDL_CreateWindow("TMXLoader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, windowFlags);

    if (window == NULL)
    {
        std::cout << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    SDL_Texture *spriteSheet = IMG_LoadTexture(renderer, "assets/spritesheet.png");
    SDL_Texture *spriteSheet1 = IMG_LoadTexture(renderer, "assets/background_sprite_sheet.png");

    TMXLoader *loader = new TMXLoader();
    loader->loadMap("testmap", "assets/testmap_v2.tmx");
    loader->printMapData("testmap");

    loader->getMap("testmap")->getTileset("defaulttileset")->printData();

    std::vector<std::string> names = loader->getMap("testmap")->getTilesetNames();

    std::cout << "\nspace" << std::endl;

    for(std::string s : names)
    {
        GlobalTiledIDS gtid;
        gtid.gid = loader->getMap("testmap")->getTileset(s)->getFirstGID();
        gtid.tileSetName = s;

        std::cout << gtid.tileSetName << std::endl;
        std::cout << gtid.gid << std::endl;

        globalTiledIDS.push_back(gtid);
    }

    bool running = true;
    SDL_Event events;
    while (running == true)
    {
        running = update(events);
        render(renderer, spriteSheet, spriteSheet1, loader);
    }

    delete loader;

    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}*/

/*struct Player {
    unsigned int wins = 0u;
    unsigned int loses = 0u;
    unsigned int draws = 0u;
    unsigned int board = 0u;
    bool turn = false;

    unsigned int games()
    {
        return wins + loses + draws;
    }

    void reset()
    {
        board = 0;
        turn = false;
    }

    void print_stats()
    {
        printf("player\n");
        printf("wins: %i \n", wins);
        printf("loses: %i \n", loses);
        printf("draw: %i \n\n", draws);
    }
};

bool is_full(const Player& p1, const Player& p2)
{
    return (p1.board | p2.board) == 32895;
}

void pick_starting_player(Player& p1, Player& p2)
{
    p1.turn = true;
}

void pass_player_turn(Player& p1, Player& p2)
{
    p1.turn != p1.turn;
    p2.turn != p2.turn;
}

Player player1 = {};
Player player2 = {};

int main(int argc, char* argv[])
{
    while(player1.games() < 1000)
    {
        // check new game
        if (player1.turn == false && player2.turn == false)
            pick_starting_player(player1,player2);
        else
            pass_player_turn(player1, player2);
        
        // take turn
        if (player1.turn)
            player1.board++;

        if (player2.turn)
            player2.board++;

        // check stalemate
        if (is_full(player1, player2))
        {
            player1.draws++;
            player2.draws++;
            player1.reset();
            player2.reset();
        }
    }

    player1.print_stats();
    player2.print_stats();

    return 0;
}*/