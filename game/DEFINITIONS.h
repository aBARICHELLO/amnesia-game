static const int GAME_WIDTH  = 1100;
static const int GAME_HEIGHT = 700;

// SFML uses pixels as coordinates while Box2D uses meters, this scale constant
// is used to convert between these two systems.
static const float SCALE = 30;

static const float HITBOX_X = 25.f;
static const float HITBOX_Y = 5.f;

// static const float ROUND_TIME = 10.f; // DEBUG
static const float ROUND_TIME = 180.f;

// TEXTURES
#define PLAYER_TEXTURE      "media/textures/player_texture.png"
#define NEON_TEXTURE        "media/textures/neon.png"
#define SPRING_TEXTURE      "media/textures/spring.png"

#define ACAI_TEXTURE        "media/textures/acai.png"
#define BEER_TEXTURE        "media/textures/beer.png"

// FONTS
#define HUD_FONT            "media/fonts/hud_font.ttf"
#define BONBON_FONT         "media/fonts/bonbon.otf"
#define SQUARE_FONT         "media/fonts/squarefont.ttf"
#define LINE_BRK_FONT       "media/fonts/line_brk.ttf"
#define ARIEL_PIXEL_FONT    "media/fonts/ariel_pixel.ttf"

// AUDIO
#define BACKGROUND_SONG     "media/audio/fishing.wav"
