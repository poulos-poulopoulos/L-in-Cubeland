//author: poulos poulopoulos

#include <cctype>
#include <climits>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <GL/freeglut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

using namespace std;





class Sun{
private:
    int x, y, z;
public:
    Sun();
    Sun(FILE *f);
    void Store(FILE *f);
    bool Move();
    void Visualize();
};





class Lamp{
private:
    int x, y, z;
    int age;
    int id;
public:
    static int const num_max = 5;
    Lamp(int x, int y, int z, int id);
    Lamp(FILE *f, int id);
    void Store(FILE *f);
    bool Weaken();
    void VisualizeLight();
    void Visualize();
};





class Cube{
private:
    int x, y, z;
    int stock;
public:
    Cube(int x, int y, int z, int stock);
    Cube(int x, int y, int z, FILE *f);
    void Store(FILE *f);
    bool DecreaseStock();
    void Move(int x, int y, int z);
    void Visualize();
};





class Camera{
private:
    float field_of_view;
    int zoom, horizontal_tilt, vertical_tilt;
protected:
    Camera(float field_of_view);
    Camera(FILE *f);
public:
    virtual void Store(FILE *f);
    bool ResetZoom();
    bool IncreaseZoom();
    bool DecreaseZoom();
    bool ResetHorizontalTilt();
    bool IncreaseHorizontalTilt();
    bool DecreaseHorizontalTilt();
    bool ResetVerticalTilt();
    bool IncreaseVerticalTilt();
    bool DecreaseVerticalTilt();
    void VisualizeTiltFrame();
    virtual bool Visualize();
};





class StillCamera : public Camera{
private:
    int x, y, z;
public:
    StillCamera(float field_of_view, int x, int y, int z);
    StillCamera(FILE *f);
    void Store(FILE *f);
    bool Visualize();
};





class Player{
public:
    enum Orientation{Y_POS, X_POS, Y_NEG, X_NEG};
private:
    class Camera : public ::Camera{
    private:
        static void (*VisualizeFigurePOV)();
        static void VisualizeLPOV();
        static void VisualizeSnowmanPOV();
        Player *player;
    public:
        static void Initialize(bool new_year);
        Camera(float field_of_view, Player *player);
        Camera(FILE *f, Player *player);
        bool Visualize();
    };
    enum SimpleMove{ROTATE_RIGHT, ROTATE_LEFT,
                    TRANSLATE_UP, TRANSLATE_DOWN,
                    TRANSLATE_FRONT, TRANSLATE_BACK};
    enum Move{NO_MOVE, FALL, TURN_RIGHT, TURN_LEFT, STEP_FRONT, STEP_BACK};
    static void (*VisualizeFigure)();
    static void VisualizeL();
    static void VisualizeSnowman();
    int x, y, z;
    Player::Orientation orientation;
    Player::SimpleMove simple_move;
    float simple_move_part;
    int simple_move_start;
    int fall_channel;
    Player::Move move;
    int lamps, cubes;
    void CalculateFrontBackPosition(int *x, int *y, int *z, int i);
    void ResetPosition();
    void StartMove(Player::Move move);
public:
    static void Initialize(bool new_year);
    Player::Camera *camera;
    Player(int x, int y, int z, Player::Orientation orientation);
    Player(FILE *f);
    ~Player();
    void Store(FILE *f);
    void ReleaseChannel(int channel);
    void HandleCollision();
    bool Moving();
    void StopMove();
    bool ContinueMove();
    void Fall();
    void TurnRight();
    void TurnLeft();
    void StepFront();
    void StepBack();
    void CreateLamp();
    void ObtainCube();
    void CreateCube();
    void PushCube();
    void DestroyCube();
    void DestroyColumn();
    void VisualizeGridLines(bool full);
    void VisualizeGridPlanes(bool full);
    void VisualizeInfo();
    void Visualize();
};





class Game{
private:
    enum GridVisibility{VISIBLE_PLAYER_CROPPED, VISIBLE_PLAYER_FULL,
                        VISIBLE_ALL, INVISIBLE};
    int light, score, stage;
    double stock_weight[5], stock_cum_prob[5];
    Game::GridVisibility grid_lines, grid_planes;
    StillCamera *still_cameras[4];
    int still_camera;
    bool cubes_collapsing;
    bool **collapse_map;
    void VisualizeWalls();
public:
    enum Event{START, POSITION_RESET, COLLISION, CUBE_CREATED, CUBE_OBTAINED,
               COLUMN_DESTROYED, PLAYER_FALLING, CUBES_COLLAPSING, NEUTRAL};
    static int start_timers, collapse_timers;
    static float a, d, r, w[5];
    static int m, n;
    static void Initialize(time_t time, int argc, char **argv);
    static bool Inside(int x, int y, int z);
    static void VisualizeGridLines(
        int x_start = 0, int y_start = 0, int z_start = 0,
        int x_size = Game::n, int y_size = Game::n, int z_size = Game::n,
        bool inside = true, bool full = true
    );
    static void VisualizeGridPlanes(
        int x_start = 0, int y_start = 0, int z_start = 0,
        int x_size = Game::n, int y_size = Game::n, int z_size = Game::n,
        bool inside = true, bool full = true
    );
    Sun *sun;
    Lamp *lamps[Lamp::num_max];
    int lamp_first, lamp_num;
    Cube ****cubes;
    Player *player;
    Camera *camera;
    bool finished;
    Game();
    Game(FILE *f);
    ~Game();
    void Store(FILE *f);
    bool ContinueStart0();
    bool ContinueStart1();
    bool ContinueStart2();
    bool Starting();
    bool Evolving();
    void Finish();
    void UpdateScore(Game::Event event);
    void UpdateLights();
    void RefreshLight(int light);
    int CreateStock();
    void StartCollapse();
    bool ContinueCollapse();
    void UseChangeStillCamera();
    void UsePlayerCamera();
    void ChangeGridLines();
    void ChangeGridPlanes();
    void VisualizeInfo();
    void Visualize();
};





class Display{
private:
    static int (*SetSwapInterval)(int);
    static bool avsync_possible;
protected:
    int id;
    Display();
    Display(FILE *f);
    void Show_();
public:
    enum Rendering{SINGLE, DOUBLE, AVSYNC, VSYNC};
    static void Initialize();
    bool showing;
    Display::Rendering rendering;
    int width, height;
    virtual void Store(FILE *f);
    virtual void LocalizePointerPosition(int *x, int *y, int *x_, int *y_);
    virtual void GlobalizePointerPosition(int *x, int *y, int *x_, int *y_);
    virtual bool Show(int pointer_x = -1, int pointer_y = -1,
                      int pointer_x_ = 0, int pointer_y_ = 0);
    virtual bool Hide();
    void PostRedisplay();
    void VisualizeDimLayer();
    void VisualizeFinished();
};





class Window : public Display{
public:
    static void Initialize();
    int x, y;
    int border_width, border_height;
    bool damaged, repair_scheduled;
    Window();
    Window(FILE *f);
    void Store(FILE *f);
    void LocalizePointerPosition(int *x, int *y, int *x_, int *y_);
    void GlobalizePointerPosition(int *x, int *y, int *x_, int *y_);
    bool Show(int pointer_x = -1, int pointer_y = -1,
              int pointer_x_ = 0, int pointer_y_ = 0);
    bool Hide();
    void Repair();
};





class FullScreen : public Display{
public:
    FullScreen();
    FullScreen(FILE *f);
    bool Show(int pointer_x = -1, int pointer_y = -1,
              int pointer_x_ = 0, int pointer_y_ = 0);
    bool Hide();
};





class Adjustment{
protected:
    Adjustment();
public:
    virtual void Toggle();
    virtual void Reset();
    virtual void Increase();
    virtual void Decrease();
};





class AutoHidable{
private:
    int timers;
    void (*TimerFunction)(int);
protected:
    bool visible;
    AutoHidable(void (*TimerFunction)(int));
    virtual bool MakeVisible();
public:
    virtual bool MakeInvisible();
    virtual void IncreaseTimers();
    void DecreaseTimers();
};





class SoundPlayer : public Adjustment, public AutoHidable{
private:
    int volume;
    bool MakeVisible();
protected:
    static char const *const sound_folder;
    bool active;
    SoundPlayer(void (*TimerFunction)(int));
    SoundPlayer(FILE *f, void (*TimerFunction)(int));
    int CalculateActualVolume();
public:
    static void Initialize();
    static void Terminate();
    void Store(FILE *f);
    void Toggle();
    void Reset();
    void Increase();
    void Decrease();
    bool Visible();
    bool MakeInvisible();
    virtual void ToggleActive();
    virtual bool ResetVolume();
    virtual bool IncreaseVolume();
    virtual bool DecreaseVolume();
    void Visualize();
};





class MusicPlayer : public SoundPlayer{
private:
    static Mix_Music *day_time;
    static Mix_Music *night_time;
    static Mix_Music *finished;
    Mix_Music *music;
public:
    static void Initialize(bool new_year);
    static void Terminate();
    MusicPlayer(void (*TimerFunction)(int));
    MusicPlayer(FILE *f, void (*TimerFunction)(int));
    ~MusicPlayer();
    void ToggleActive();
    bool ResetVolume();
    bool IncreaseVolume();
    bool DecreaseVolume();
    void Play();
};





class EffectPlayer : public SoundPlayer{
private:
    static Mix_Chunk *creation;
    static Mix_Chunk *stock;
    static Mix_Chunk *lamp;
    static Mix_Chunk *collapse;
    static Mix_Chunk *fall;
    static Mix_Chunk *disaster;
    static Mix_Chunk *simple;
public:
    static void Initialize(bool new_year);
    static void Terminate();
    EffectPlayer(void (*TimerFunction)(int));
    EffectPlayer(FILE *f, void (*TimerFunction)(int));
    ~EffectPlayer();
    int Play(Game::Event event);
    void Stop(int channel, int time = 0);
};





class Pointer : public Adjustment, public AutoHidable{
private:
    bool active;
    int tolerance;
    bool MakeVisible();
public:
    int x, y, x_, y_;
    Pointer(void (*TimerFunction)(int));
    Pointer(FILE *f, void (*TimerFunction)(int));
    void Store(FILE *f);
    void Toggle();
    void Reset();
    void Increase();
    void Decrease();
    bool MakeInvisible();
    void IncreaseTimers();
    void Show();
    void Hide();
    bool Positioned();
    bool Active();
    void ToggleActive();
    int Tolerance();
    bool ResetTolerance();
    bool IncreaseTolerance();
    bool DecreaseTolerance();
};





class Zoom : public Adjustment{
public:
    void Reset();
    void Increase();
    void Decrease();
};





class HorizontalTilt : public Adjustment{
public:
    void Reset();
    void Increase();
    void Decrease();
};





class VerticalTilt : public Adjustment{
public:
    void Reset();
    void Increase();
    void Decrease();
};





class GameIO{
private:
    int length, cursor, visible_start;
protected:
    char *filename;
    GameIO();
public:
    ~GameIO();
    virtual void DoIO();
    bool MoveLeft();
    bool MoveRight();
    bool GoToStart();
    bool GoToEnd();
    bool Insert(char c);
    bool Delete();
    void Visualize();
};





class GameLoader : public GameIO{
public:
    void DoIO();
};





class GameStorer : public GameIO{
public:
    void DoIO();
};





class Configuration{
private:
    bool game_info, player_info;
public:
    static void Initialize(bool new_year);
    static void Terminate();
    Window *window;
    FullScreen *full_screen;
    Display *display;
    MusicPlayer *music_player;
    EffectPlayer *effect_player;
    Pointer *pointer;
    Zoom *zoom;
    HorizontalTilt *horizontal_tilt;
    VerticalTilt *vertical_tilt;
    Adjustment *adjustment;
    GameIO *game_i_o;
    Configuration();
    Configuration(FILE *f);
    ~Configuration();
    void Store(FILE *f);
    void ToggleGameInfo();
    void TogglePlayerInfo();
    void ChangeDisplay();
    void SetAdjustment(Adjustment *adjustment);
    void CreateGameLoader();
    void CreateGameStorer();
    void ExitGameIO();
    void Visualize();
};





extern char const *const storage_folder;
extern Game *game;
extern Configuration *cfg;
void Initialize(int argc, char **argv);
void Terminate();
void StartGame();
bool LoadGame(char const *filename = "state");
bool StoreGame(char const *filename = "state");
int ElapsedTime();
void ActivateAnimation();
void SetTimer(void (*TimerFunction)(int), int time = 0);
void WindowInitDisplayFunction();
void InitDisplayFunction();
void MainDisplayFunction();
void InitPositionFunction(int x, int y);
void MainPositionFunction(int x, int y);
void ReshapeFunction(int width, int height);
void IOKeyboardFunction(unsigned char key, int x, int y);
void MainKeyboardFunction(unsigned char key, int x, int y);
void IOSpecialFunction(int key, int x, int y);
void MainSpecialFunction(int key, int x, int y);
void MouseFunction(int button, int state, int x, int y);
void MouseWheelFunction(int wheel, int direction, int x, int y);
void EntryFunction(int state);
void InactivePassiveMotionFunction(int x, int y);
void ActivePassiveMotionFunction(int x, int y);
void IdleFunction();
void InitRepairTimerFunction(int _);
void MainRepairTimerFunction(int _);
void MusicPlayerTimerFunction(int _);
void EffectPlayerTimerFunction(int _);
void PointerTimerFunction(int _);
void StoreTimerFunction(int _);
void Start0TimerFunction(int _);
void Start1TimerFunction(int _);
void Start2TimerFunction(int _);
void CollapseTimerFunction(int _);
