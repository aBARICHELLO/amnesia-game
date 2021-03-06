static const int GAME_WIDTH = 1366;
static const int GAME_HEIGHT = 768;

static const int CONTROLLER_THRESHOLD = 25; // Controller deadzone

// SFML uses pixels as coordinates while Box2D uses meters, this scale constant
// is used to convert between these two systems.
static const float SCALE = 30;

static const float HITBOX_X = 25.f;
static const float HITBOX_Y = 5.f;

// static const float ROUND_TIME = 10.f; // DEBUG
static const float ROUND_TIME = 180.f;

// TEXTURES
#define PLAYER_TEXTURE        "media/textures/player_texture.png"
#define INVERTED_TEXTURE      "media/textures/inverted_player.png"
#define IMMUNITY_TEXTURE      "media/textures/immunity.png"
#define NEON_TEXTURE          "media/textures/block.png"
#define DESERT_BLOCK_TEXTURE  "media/textures/desert_block.png"
#define FOG_TEXTURE           "media/textures/fog.png"
#define SPRING_TEXTURE        "media/textures/spring.png"
#define PACKAGE_TEXTURE       "media/textures/package.png"
#define TELEPORT_TEXTURE      "media/textures/teleport.png"

// BACKGROUNDS
#define AMNESIA_LOGO          "media/textures/amnesia.png"
#define DESERT_TEXTURE        "media/textures/desert.png"
#define CONTROLS_TEXTURE      "media/textures/controls.png"

#define ACAI_TEXTURE          "media/textures/acai.png"
#define BEER_TEXTURE          "media/textures/beer.png"
#define VODKA_TEXTURE         "media/textures/vodka.png"
#define ABSINTO_TEXTURE       "media/textures/absinto.png"
#define TEQUILA_TEXTURE       "media/textures/tequila.png"

// FONTS
#define HUD_FONT              "media/fonts/hud_font.ttf"
#define BONBON_FONT           "media/fonts/bonbon.otf"
#define SQUARE_FONT           "media/fonts/squarefont.ttf"
#define LINE_BRK_FONT         "media/fonts/line_brk.ttf"
#define ARIEL_PIXEL_FONT      "media/fonts/ariel_pixel.ttf"
#define BANKSIA_FONT          "media/fonts/banksia.ttf"

// AUDIO
#define BACKGROUND_SONG1      "media/audio/background1.wav"
#define BACKGROUND_SONG2      "media/audio/background2.wav"
#define BACKGROUND_SONG3      "media/audio/background3.wav"
#define SPRING_SOUND          "media/audio/spring.wav"
#define SQUASH_SOUND          "media/audio/squash.ogg"
