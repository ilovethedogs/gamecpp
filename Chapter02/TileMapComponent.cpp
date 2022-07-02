#include "TileMapComponent.h"

size_t read_data(char const* const filename, std::function<char* (size_t const)> allocator) {
    size_t readbytes{ 0 };
    std::ifstream ifile(filename, std::ios::ate | std::ios::binary);
    if (ifile.is_open()) {
        auto length{ static_cast<size_t> (ifile.tellg()) };
        ifile.seekg(0, std::ios_base::beg);

        auto buffer{ allocator(length) };
        try {
            ifile.read(buffer, length);
            readbytes = static_cast<size_t> (ifile.gcount());
        }
        catch (std::ios_base::failure&) {
            // handle error
        }
        ifile.close();
    }
    return readbytes;
}

void read_csv(char const* const filename, std::array<int, TILE_SIZE>& a) {
    auto input{ std::make_unique<std::vector<unsigned char>>() };
    if (read_data(filename, [&input](size_t const length) {
        input->resize(length);
        return reinterpret_cast<char*> (input->data()); })
        ) {
        // successfully read file to input vector
        size_t idx{ 0 };
        int tmp{ 0 };
        int negative{ 1 };
        for (auto& c : *input) {
            switch (c) {
            case ',': case '\r':
                tmp *= negative;
                negative = 1;
                a[idx++] = tmp;
                tmp = 0;
                break;
            case '\n':
                a[idx++] = -2;
                break;
            case '-':
                negative = -1;
                break;
            default:
                tmp *= 10 * (tmp != 0) + 1 * (tmp == 0);
                tmp += static_cast<int>(c - '0');
            }
        }
    }
}

TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
    :SpriteComponent(owner, drawOrder)
{
}

void TileMapComponent::Draw(SDL_Renderer* renderer) {
    SDL_Rect t{};
    SDL_Rect r{};
    t.w = t.h = r.w = r.h =  32;
    for (const auto& i : mTiles) {
		r.x = static_cast<int>(mOwner->GetPosition().x);
		r.y = static_cast<int>(mOwner->GetPosition().y);
        for (const auto n : i) {
            switch (n) {
            case -1:
                r.x += 32;
                break;
            case -2:
                r.x = static_cast<int>(mOwner->GetPosition().x);
                r.y += 32;
                break;
            default:
                t.x = (n % 8) * 32;
                t.y = static_cast<int>(n / 8) * 32;
				SDL_RenderCopyEx(renderer,
					mTexture,
					&t,
					&r,
					0.0,
					nullptr,
					SDL_FLIP_NONE);
                r.x += 32;
            }
        }
    }
}

void TileMapComponent::readTiles(char const* const filename) {
    std::array<int, TILE_SIZE> a{};
    read_csv(filename, a);
    mTiles.emplace_back(a);
}
