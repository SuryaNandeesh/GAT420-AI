#include "GameOfLife.h"
#include "Texture.h"
#include "Random.h"

bool GameOfLife::Initialize()
{
	bufferA = std::vector<uint8_t>(size.x * size.y);
	bufferB = std::vector<uint8_t>(size.x * size.y);
    return true;
}

void GameOfLife::Step()
{
	//<buffer determined if frame is even or odd>
	frame++;
	std::vector<uint8_t>& readBuffer = 
		frame % 2 ? bufferA : bufferB; 
	std::vector<uint8_t>& writeBuffer =
		frame % 2 ? bufferB : bufferA;

    // update buffer
    // clear write buffer
    std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            int weight = 0;

            // read surrounding 8 cells, accumulate weight
            // XXX
            // X0X
            // XXX
            weight += Read<uint8_t>(readBuffer, x - 1, y - 1); // top left
            weight += Read<uint8_t>(readBuffer, x    , y - 1); // top middle
            weight += Read<uint8_t>(readBuffer, x + 1, y - 1); // top right
            weight += Read<uint8_t>(readBuffer, x - 1, y    ); // middle left
            //weight += Read<uint8_t>(readBuffer, x  , y    ); // middle middle
            weight += Read<uint8_t>(readBuffer, x + 1, y    ); // middle right
            weight += Read<uint8_t>(readBuffer, x - 1, y + 1); // bottom left
            weight += Read<uint8_t>(readBuffer, x    , y + 1); // bottom middle
            weight += Read<uint8_t>(readBuffer, x + 1, y + 1); // bottom right

            // game of life rules
           // if cell is alive, update
            if (Read<uint8_t>(readBuffer, x, y))
            {
                //if (<rule>) Write<uint8_t>(<write buffer at x, y>);
                if (weight > 3 || weight < 2) {
                    Write<uint8_t>(writeBuffer, x, y, 0);
                    
                }
                else 
                {
                    Write<uint8_t>(writeBuffer, x, y, 1);
                }
            }
            else
            {
                // if cell is dead, update
                if (weight == 3) {
                    Write<uint8_t>(writeBuffer, x, y, 1);
                }
                else
                {
                    Write<uint8_t>(writeBuffer, x, y, 0);
                }
            }
        }
    }

    // convert buffer data format to color buffer
    std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v)
        {
            { return  v ? 0xffffffff : 0; }
        });

    // copy color buffer to texture
    texture->Copy(color_buffer);
}

void GameOfLife::KeyDown(SDL_Keycode keycode)
{
    if (keycode == SDLK_r)
    {
        // write random 'alive' cells to buffer
        std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;
        for (int i = 0; i < writeBuffer.size(); i++) writeBuffer[i] = (random(0, 10) == 0) ? 1 : 0;
    }
}