#include "libretro.hpp"
#include "SDL.h"
#include "../Game.hpp"
#include "Input.hpp"
#include <cstring>
#include <memory>

static const unsigned FRAMEBUFFER_WIDTH = 960;
static const unsigned FRAMEBUFFER_HEIGHT = 540;

std::string core_path;
std::string system_dir;
const auto inputIdMap = PlayerIdExtensions::HumanPlayers();
std::vector<Input> input;
std::unique_ptr<Game> game;
std::unique_ptr<Assets> assets;
SDL_Surface* framebuffer = nullptr;
SDL_Renderer* renderer = nullptr;
bool audioEnabled = true;
#define AUDIO_FRAMES (44100 / Game::TARGET_FPS)
int16_t buffer[AUDIO_FRAMES * 2];

static bool use_audio_cb;

// Callbacks
static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;

static double delta_time_ms;

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_cheat_reset(void)
{

}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{

}

static void audio_callback(void)
{
    if (!audioEnabled)
        return;
    for (size_t i = 0; i < AUDIO_FRAMES; )
    {
        size_t written = audio_batch_cb(buffer + 2 * i, AUDIO_FRAMES - i);
        i += written;
    }
}

static void audio_set_state(bool enable)
{
    audioEnabled = true;
}

static void SDLPostMixCallback(void *udata, Uint8 *stream, int len) {
    // Copy from SDL_Mixer to audio buffer
    for (int i = 0; i < std::min(len, AUDIO_FRAMES * 2); ++i) {
        buffer[i] = (int16_t) stream[i];
    }
}

bool retro_load_game(const struct retro_game_info *info)
{
    struct retro_audio_callback cb = { audio_callback, audio_set_state };
    use_audio_cb = environ_cb(RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK, &cb);

    input = {{}, {}, {}, {}};
    std::cout << "Setting up a maximum of " << input.size() << " controllers." << std::endl;
    log_cb(RETRO_LOG_INFO, "VanHunksDefence: ", "Logging something: %d.\n", 1);
    log_cb(RETRO_LOG_INFO, "VanHunksDefence: ", ("core path = " + core_path).c_str());
    log_cb(RETRO_LOG_INFO, "VanHunksDefence: ", ("system dir = " + system_dir).c_str());

    if (renderer == nullptr) {
        SDL_Init(SDL_INIT_VIDEO);
        framebuffer = SDL_CreateRGBSurface(SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 32, 0, 0, 0, 0);
        renderer = SDL_CreateSoftwareRenderer(framebuffer);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        Mix_SetPostMix(SDLPostMixCallback, nullptr);
    }

    assets = std::make_unique<Assets>(renderer);
    game = std::make_unique<Game>(assets.get(), FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT); //, core_path, system_dir);
    return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    return false;
}

void retro_unload_game(void)
{

}

unsigned retro_get_region(void)
{
    return RETRO_REGION_NTSC;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{

}

void *retro_get_memory_data(unsigned id)
{
    return nullptr;
}

size_t retro_get_memory_size(unsigned id)
{
    return 0;
}

size_t retro_serialize_size(void)
{
    return 0;
}

bool retro_serialize(void *data, size_t size)
{
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{
    return false;
}

void retro_deinit(void)
{
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
    // Start without rom
    bool no_rom = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);

    // Delta time setup
    retro_usec_t time_reference = 1000000 / Game::TARGET_FPS;
    auto frame_time_cb = [](retro_usec_t usec) { delta_time_ms = usec / 1000.0; };
    struct retro_frame_time_callback frame_cb = { frame_time_cb, time_reference };
    cb(RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK, &frame_cb);

    // retro_set_environment seems to get called multiple times, but only the first time contains the right path.
    if (core_path.empty()) {
        char *name = new char[PATH_MAX];  // TODO: Memory leak, but I can't delete[] it?
        cb(RETRO_ENVIRONMENT_GET_LIBRETRO_PATH, &name);
        core_path = std::string(name);
        core_path = core_path.substr(0, core_path.find_last_of('/') + 1);
        std::cout << "core path: " << core_path << std::endl;
        cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &name);
        system_dir = std::string(name);
        if (system_dir.find('/') != std::string::npos)
            system_dir += '/';
        else if (system_dir.find('\\') != std::string::npos)
            system_dir += '\\';
        std::cout << "system dir path: " << system_dir << std::endl;
    }
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }



void retro_init(void)
{
    struct retro_log_callback log{};
    if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
        log_cb = log.log;
    else
        log_cb = nullptr;
}

void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = Game::NAME;
    info->library_version = Game::VERSION;
    info->need_fullpath = false;
    info->valid_extensions = "zip";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
    int pixel_format = RETRO_PIXEL_FORMAT_XRGB8888;

    memset(info, 0, sizeof(*info));
    info->timing.fps            = Game::TARGET_FPS;
    info->timing.sample_rate    = 44100;
    info->geometry.base_width   = FRAMEBUFFER_WIDTH;
    info->geometry.base_height  = FRAMEBUFFER_HEIGHT;
    info->geometry.max_width    = FRAMEBUFFER_WIDTH;
    info->geometry.max_height   = FRAMEBUFFER_HEIGHT;
    info->geometry.aspect_ratio = (float) FRAMEBUFFER_WIDTH / (float) FRAMEBUFFER_HEIGHT;

    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

void retro_reset(void) { }

std::vector<Input> retropad_map_input(unsigned long num_controllers, std::vector<Input> previous_input) {
    // Poll input
    input_poll_cb();

    std::vector<Input> controller_states;
    for (unsigned int player = 0; player < num_controllers; ++player) {
        Input input = {};

        input.fire = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) == 1;
        input.back = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) == 1;
        input.right = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) == 1;
        input.down = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) == 1;
        input.left = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) == 1;
        input.up = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) == 1;
        input.start = input_state_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START) == 1;

        input.fire_pressed = !previous_input[player].fire && input.fire;
        input.back_pressed = !previous_input[player].back && input.back;
        input.right_pressed = !previous_input[player].right && input.right;
        input.down_pressed = !previous_input[player].down && input.down;
        input.left_pressed = !previous_input[player].left && input.left;
        input.up_pressed = !previous_input[player].up && input.up;
        input.start_pressed = !previous_input[player].start && input.start;

        input.fire_released = previous_input[player].fire && !input.fire;
        input.back_released = previous_input[player].back && !input.back;
        input.right_released = previous_input[player].right && !input.right;
        input.down_released = previous_input[player].down && !input.down;
        input.left_released = previous_input[player].left && !input.left;
        input.up_released = previous_input[player].up && !input.up;
        input.start_released = previous_input[player].start && !input.start;

        controller_states.push_back(input);
    }
    return controller_states;
}


void retro_run(void)
{
    // Input
    input = retropad_map_input(input.size(), input);

    for (int i = 0; i < input.size(); ++i) {
        if (input[i].start_pressed)
            game->PressStart(inputIdMap[i]);
        if (input[i].fire_pressed)
            game->PressFire(inputIdMap[i]);
        if (input[i].left)
            game->PressLeft(inputIdMap[i], delta_time_ms);
        if (input[i].right)
            game->PressRight(inputIdMap[i], delta_time_ms);
        if (input[i].up)
            game->PressUp(inputIdMap[i], delta_time_ms);
        if (input[i].down)
            game->PressDown(inputIdMap[i], delta_time_ms);
    }

    game->Update(delta_time_ms);

    SDL_RenderClear(renderer);
    game->Render(renderer);

    video_cb(framebuffer->pixels, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, sizeof(uint32_t) * FRAMEBUFFER_WIDTH);

    SDL_RenderPresent(renderer);
}