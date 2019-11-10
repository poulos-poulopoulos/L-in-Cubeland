//author: poulos poulopoulos

#include "LiC.h"





Sun::Sun(){

    this->x = this->y = -2 * Game::n;
    this->z = Game::n;

}

Sun::Sun(FILE *f){

    fscanf(f, "%d%d%d", &this->x, &this->y, &this->z);
    fgetc(f);

}

void Sun::Store(FILE *f){

    fprintf(f, "%d %d %d\n", this->x, this->y, this->z);

}

bool Sun::Move(){

    if(this->x == 3 * Game::n){
        delete this;
        return false;
    }
    if(this->x < 0) this->z++;
    else if(this->x >= Game::n) this->z--;
    this->x++;
    this->y++;
    return true;

}

void Sun::Visualize(){

    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(this->x, this->z, -this->y);
    glutSolidSphere(Game::m / 2.0, 50, 50);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    float const position[] = {this->x, this->z, -this->y, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    static float const ambient[] = {0.2, 0.2, 0.2, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glEnable(GL_LIGHT0);

}





Lamp::Lamp(int x, int y, int z, int id){

    this->x = x;
    this->y = y;
    this->z = z;
    this->age = 0;
    this->id = id;

}

Lamp::Lamp(FILE *f, int id){

    fscanf(f, "%d%d%d%d", &this->x, &this->y, &this->z, &this->age);
    fgetc(f);
    this->id = id;

}

void Lamp::Store(FILE *f){

    fprintf(f, "%d %d %d %d\n", this->x, this->y, this->z, this->age);

}

bool Lamp::Weaken(){

    if(this->age == 4 * Game::m){
        delete this;
        return false;
    }
    this->age++;
    return true;

}

void Lamp::VisualizeLight(){

    float const position[] = {this->x + 0.5, this->z + 0.5, -(this->y + 0.5), 1.0};
    glLightfv(GL_LIGHT1 + this->id, GL_POSITION, position);
    static float const light[] = {1.0, 0.5, 0.0, 1.0};
    glLightfv(GL_LIGHT1 + this->id, GL_DIFFUSE, light);
    static float const ambient[] = {0.05, 0.025, 0.0, 1.0};
    glLightfv(GL_LIGHT1 + this->id, GL_AMBIENT, ambient);
    float const strength = 1.0 - pow(this->age / (4.0 * Game::m + 1.0), 5.0);
    glLightf(GL_LIGHT1 + this->id,
             GL_CONSTANT_ATTENUATION, 0.5 / strength);
    glLightf(GL_LIGHT1 + this->id,
             GL_QUADRATIC_ATTENUATION, 1.5 / (Game::m * Game::m * strength));
    glEnable(GL_LIGHT1 + this->id);

}

void Lamp::Visualize(){

    static float const light[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT1 + this->id, GL_AMBIENT, light);
    glColor3f(1.0, 0.5, 0.0);
    glPushMatrix();
    glTranslatef(this->x + 0.5, this->z + 0.5, -(this->y + 0.5));
    glutSolidSphere(0.5, 50, 50);
    glPopMatrix();
    static float const ambient[] = {0.05, 0.025, 0.0, 1.0};
    glLightfv(GL_LIGHT1 + this->id, GL_AMBIENT, ambient);

}





Cube::Cube(int x, int y, int z, int stock){

    this->x = x;
    this->y = y;
    this->z = z;
    this->stock = stock;

}

Cube::Cube(int x, int y, int z, FILE *f){

    this->x = x;
    this->y = y;
    this->z = z;
    fscanf(f, "%d", &this->stock);
    fgetc(f);

}

void Cube::Store(FILE *f){

    fprintf(f, "%d\n", this->stock);

}

bool Cube::DecreaseStock(){

    if(this->stock == 0) return false;
    this->stock--;
    return true;

}

void Cube::Move(int x, int y, int z){

    this->x = x;
    this->y = y;
    this->z = z;

}

void Cube::Visualize(){

    if(this->x == Game::m && this->y == Game::m && this->z == 0)
        glColor3f(1.0, 1.0, 1.0);
    else if(this->stock == 0)
        glColor3f(1.0, 0.5, 0.5);
    else if(this->stock == 1)
        glColor3f(1.0, 0.0, 0.0);
    else if(this->stock == 2)
        glColor3f(0.5, 0.0, 0.0);
    else if(this->stock == 3)
        glColor3f(0.25, 0.0, 0.0);
    else if(this->stock == 4)
        glColor3f(0.125, 0.0, 0.0);

    glPushMatrix();
    glTranslatef(this->x, this->z, -this->y);
    glBegin(GL_QUADS);
    //top
    if(this->z != Game::n - 1 && game->cubes[this->x][this->y][this->z + 1] == NULL){
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(0.0, 1.0, -1.0);
    }
    //bottom
    if(this->z != 0 && game->cubes[this->x][this->y][this->z - 1] == NULL){
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, -1.0);
        glVertex3f(1.0, 0.0, -1.0);
        glVertex3f(1.0, 0.0, 0.0);
    }
    //back
    if(this->y != 0 && game->cubes[this->x][this->y - 1][this->z] == NULL){
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
    }
    //front
    if(this->y != Game::n - 1 && game->cubes[this->x][this->y + 1][this->z] == NULL){
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(0.0, 0.0, -1.0);
        glVertex3f(0.0, 1.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(1.0, 0.0, -1.0);
    }
    //left
    if(this->x != 0 && game->cubes[this->x - 1][this->y][this->z] == NULL){
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0, -1.0);
        glVertex3f(0.0, 0.0, -1.0);
    }
    //right
    if(this->x != Game::n - 1 && game->cubes[this->x + 1][this->y][this->z] == NULL){
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(1.0, 1.0, 0.0);
    }
    glEnd();
    glPopMatrix();

}





Camera::Camera(float field_of_view){

    this->field_of_view = field_of_view;
    this->zoom = this->horizontal_tilt = this->vertical_tilt = 0;

}

Camera::Camera(FILE *f){

    fscanf(f, "%f%d%d%d",
           &this->field_of_view,
           &this->zoom, &this->horizontal_tilt, &this->vertical_tilt);
    fgetc(f);

}

void Camera::Store(FILE *f){

    fprintf(f, "%f %d %d %d\n",
            this->field_of_view,
            this->zoom, this->horizontal_tilt, this->vertical_tilt);

}

bool Camera::ResetZoom(){

    if(game->Starting() || game->finished ||
       this->zoom == 0)
        return false;
    this->zoom = 0;
    return true;

}

bool Camera::IncreaseZoom(){

    if(game->Starting() || game->finished ||
       this->zoom == 30)
        return false;
    this->zoom++;
    return true;

}

bool Camera::DecreaseZoom(){

    if(game->Starting() || game->finished ||
       this->zoom == 0)
        return false;
    this->zoom--;
    return true;

}

bool Camera::ResetHorizontalTilt(){

    if(game->Starting() || game->finished ||
       this->horizontal_tilt == 0)
        return false;
    this->horizontal_tilt = 0;
    return true;

}

bool Camera::IncreaseHorizontalTilt(){

    if(game->Starting() || game->finished ||
       this->horizontal_tilt == 75)
        return false;
    this->horizontal_tilt++;
    return true;

}

bool Camera::DecreaseHorizontalTilt(){

    if(game->Starting() || game->finished ||
       this->horizontal_tilt == -75)
        return false;
    this->horizontal_tilt--;
    return true;

}

bool Camera::ResetVerticalTilt(){

    if(game->Starting() || game->finished ||
       this->vertical_tilt == 0)
        return false;
    this->vertical_tilt = 0;
    return true;

}

bool Camera::IncreaseVerticalTilt(){

    if(game->Starting() || game->finished ||
       this->vertical_tilt == 75)
        return false;
    this->vertical_tilt++;
    return true;

}

bool Camera::DecreaseVerticalTilt(){

    if(game->Starting() || game->finished ||
       this->vertical_tilt == -75)
        return false;
    this->vertical_tilt--;
    return true;

}

void Camera::VisualizeTiltFrame(){

    int const x = cfg->pointer->x, y = cfg->display->height - cfg->pointer->y;
    int const step = cfg->pointer->Tolerance();
    glEnable(GL_BLEND);
    glColor4f(0.5, 0.5, 0.5, 0.25);
    //left
    glRecti(x - (75 + this->horizontal_tilt + 1) * step,
            y - (75 + this->vertical_tilt + 1) * step,
            x - (75 + this->horizontal_tilt) * step,
            y + (75 - this->vertical_tilt + 1) * step);
    //right
    glRecti(x + (75 - this->horizontal_tilt) * step,
            y - (75 + this->vertical_tilt + 1) * step,
            x + (75 - this->horizontal_tilt + 1) * step,
            y + (75 - this->vertical_tilt + 1) * step);
    //bottom
    glRecti(x - (75 + this->horizontal_tilt) * step,
            y - (75 + this->vertical_tilt + 1) * step,
            x + (75 - this->horizontal_tilt) * step,
            y - (75 + this->vertical_tilt) * step);
    //top
    glRecti(x - (75 + this->horizontal_tilt) * step,
            y + (75 - this->vertical_tilt) * step,
            x + (75 - this->horizontal_tilt) * step,
            y + (75 - this->vertical_tilt + 1) * step);
    glDisable(GL_BLEND);

}

bool Camera::Visualize(){

    static float const zoom_base = pow(6.0, -1.0 / 30.0);
    static float const dist_max = 3.0 * Game::n * sqrt(3.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(this->field_of_view * pow(zoom_base, this->zoom),
                   (float)cfg->display->width / cfg->display->height,
                   0.01, dist_max);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.05);
    gluLookAt(0.0, 0.0, 0.0,
              this->horizontal_tilt, this->vertical_tilt, -75.0,
              0.0, 1.0, 0.0);
    return true;

}





StillCamera::StillCamera(float field_of_view, int x, int y, int z) :
Camera(field_of_view){

    this->x = x;
    this->y = y;
    this->z = z;

}

StillCamera::StillCamera(FILE *f) :
Camera(f){

    fscanf(f, "%d%d%d", &this->x, &this->y, &this->z);
    fgetc(f);

}

void StillCamera::Store(FILE *f){

    this->Camera::Store(f);
    fprintf(f, "%d %d %d\n", this->x, this->y, this->z);

}

bool StillCamera::Visualize(){

    if(game->cubes[this->x - (this->x > Game::m)]
                  [this->y - (this->y > Game::m)]
                  [this->z - (this->z > Game::m)] != NULL){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return false;
    }
    this->Camera::Visualize();
    glTranslatef(0.0, 0.0, 0.1);
    gluLookAt(this->x, this->z, -this->y,
              Game::m + 0.5, Game::m + 0.5, -(Game::m + 0.5),
              0.0, 1.0, 0.0);
    return true;

}





void (*Player::Camera::VisualizeFigurePOV)();

void Player::Camera::Initialize(bool new_year){

    Player::Camera::VisualizeFigurePOV = (new_year ?
                                          Player::Camera::VisualizeSnowmanPOV :
                                          Player::Camera::VisualizeLPOV);

}

void Player::Camera::VisualizeLPOV(){

    glTranslatef(0.0, 0.0, 0.1);

}

void Player::Camera::VisualizeSnowmanPOV(){

    glTranslatef(0.0, -0.35, 0.3);

}

Player::Camera::Camera(float field_of_view, Player *player) :
::Camera(field_of_view){

    this->player = player;

}

Player::Camera::Camera(FILE *f, Player *player) :
::Camera(f){

    this->player = player;

}

bool Player::Camera::Visualize(){

    this->::Camera::Visualize();

    Player::Camera::VisualizeFigurePOV();

    if(this->player->Moving())
        if(this->player->simple_move == Player::ROTATE_RIGHT)
            glRotatef(this->player->simple_move_part * 90.0, 0.0, 1.0, 0.0);
        else if(this->player->simple_move == Player::ROTATE_LEFT)
            glRotatef(-this->player->simple_move_part * 90.0, 0.0, 1.0, 0.0);
        else if(this->player->simple_move == Player::TRANSLATE_UP)
            glTranslatef(0.0, -this->player->simple_move_part, 0.0);
        else if(this->player->simple_move == Player::TRANSLATE_DOWN)
            glTranslatef(0.0, this->player->simple_move_part, 0.0);
        else if(this->player->simple_move == Player::TRANSLATE_FRONT)
            glTranslatef(0.0, 0.0, this->player->simple_move_part);
        else if(this->player->simple_move == Player::TRANSLATE_BACK)
            glTranslatef(0.0, 0.0, -this->player->simple_move_part);

    if(this->player->orientation == Player::Y_POS)
        gluLookAt(this->player->x + 0.5, this->player->z + 1.5, -(this->player->y + 0.5),
                  this->player->x + 0.5, this->player->z + 1.5, -Game::n,
                  0.0, 1.0, 0.0);
    else if(this->player->orientation == Player::X_POS)
        gluLookAt(this->player->x + 0.5, this->player->z + 1.5, -(this->player->y + 0.5),
                  Game::n, this->player->z + 1.5, -(this->player->y + 0.5),
                  0.0, 1.0, 0.0);
    else if(this->player->orientation == Player::Y_NEG)
        gluLookAt(this->player->x + 0.5, this->player->z + 1.5, -(this->player->y + 0.5),
                  this->player->x + 0.5, this->player->z + 1.5, 0.0,
                  0.0, 1.0, 0.0);
    else if(this->player->orientation == Player::X_NEG)
        gluLookAt(this->player->x + 0.5, this->player->z + 1.5, -(this->player->y + 0.5),
                  0.0, this->player->z + 1.5, -(this->player->y + 0.5),
                  0.0, 1.0, 0.0);

    return true;

}





void (*Player::VisualizeFigure)();

void Player::Initialize(bool new_year){

    Player::Camera::Initialize(new_year);
    Player::VisualizeFigure = (new_year ?
                               Player::VisualizeSnowman :
                               Player::VisualizeL);

}

void Player::VisualizeL(){

    glColor3f(0.1, 0.1, 0.1);
    glTranslatef(0.0, 0.25, 0.25);
    //front
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.5);
    glutSolidSphere(0.25, 50, 50);
    glutSolidCylinder(0.25, 0.5, 50, 50);
    glPopMatrix();
    //back
    glPushMatrix();
    glTranslatef(0.0, 1.5, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidSphere(0.25, 50, 50);
    glutSolidCylinder(0.25, 1.5, 50, 150);
    glPopMatrix();
    //joint
    glRotatef(45.0, 1.0, 0.0, 0.0);
    glutSolidSphere(0.25, 50, 50);

}

void Player::VisualizeSnowman(){
    //bottom
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.5, 0.0);
    glutSolidSphere(0.5, 50, 50);
    //torso
    glTranslatef(0.0, 0.375 + 0.35, 0.0);
    glutSolidSphere(0.35, 50, 50);
    //buttons
    glColor3f(0.0, 0.0, 0.0);
    for(float a = -20.0; a <= 40.0; a += 20.0){
        glPushMatrix();
        glRotatef(a, 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, -0.35);
        glutSolidSphere(0.01, 10, 10);
        glPopMatrix();
    }
    //arms
    glColor3f(0.5, 0.25, 0.1);
    for(float a = -90.0; a <= 90.0; a += 180.0){
        glPushMatrix();
        glRotatef(a, 0.0, 1.0, 0.0);
        glRotatef(-45.0, 1.0, 0.0, 0.0);
        glutSolidCylinder(0.01, 0.69, 10, 1);
        glTranslatef(0.0, 0.0, 0.49);
        glPushMatrix();
        glRotatef(30.0, 1.0, 0.0, 0.0);
        glutSolidCylinder(0.01, 0.15, 10, 1);
        glPopMatrix();
        glTranslatef(0.0, 0.0, 0.05);
        glRotatef(-30.0, 1.0, 0.0, 0.0);
        glutSolidCylinder(0.01, 0.1, 10, 1);
        glPopMatrix();
    }
    //head
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, 0.275 + 0.25, 0.0);
    glutSolidSphere(0.25, 50, 50);
    //nose
    glColor3f(1.0, 0.5, 0.0);
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glutSolidCone(0.05, 0.4, 25, 25);
    glPopMatrix();
    //mouth
    glColor3f(0.5, 0.25, 0.1);
    glPushMatrix();
    glRotatef(-20.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -0.248);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.05);
    glutSolidCylinder(0.005, 0.1, 10, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.048);
    glRotatef(-135.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(0.005, 0.025, 10, 1);
    glPopMatrix();
    glTranslatef(0.0, 0.0, 0.048);
    glRotatef(-45.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(0.005, 0.025, 10, 1);
    glPopMatrix();
    //eyes
    glColor3f(0.0, 0.0, 0.0);
    for(float a = -20.0; a <= 20.0; a += 40.0){
        glPushMatrix();
        glRotatef(a, 0.0, 1.0, 0.0);
        glRotatef(20.0, 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, -0.25);
        glutSolidSphere(0.02, 25, 25);
        glPopMatrix();
    }

}

Player::Player(int x, int y, int z, Player::Orientation orientation){

    this->x = x;
    this->y = y;
    this->z = z;
    this->orientation = orientation;
    this->fall_channel = -1;
    this->move = Player::NO_MOVE;
    this->lamps = this->cubes = 0;
    this->camera = new Player::Camera(90.0, this);

}

Player::Player(FILE *f){

    fscanf(f, "%d%d%d%d%d%d",
           &this->x, &this->y, &this->z, &this->orientation,
           &this->lamps, &this->cubes);
    fgetc(f);
    this->fall_channel = -1;
    this->move = Player::NO_MOVE;
    this->camera = new Player::Camera(f, this);

}

Player::~Player(){

    this->StopMove();
    delete this->camera;

}

void Player::Store(FILE *f){

    fprintf(f, "%d %d %d %d %d %d\n",
            this->x, this->y, this->z, this->orientation,
            this->lamps, this->cubes);
    this->camera->Store(f);

}

void Player::CalculateFrontBackPosition(int *x, int *y, int *z, int i){

    *x = this->x;
    *y = this->y;
    *z = this->z;
    if(this->orientation == Player::X_NEG) *x -= i;
    else if(this->orientation == Player::X_POS) *x += i;
    else if(this->orientation == Player::Y_NEG) *y -= i;
    else if(this->orientation == Player::Y_POS) *y += i;

}

void Player::ResetPosition(){

    game->UpdateScore(Game::POSITION_RESET);
    cfg->effect_player->Play(Game::POSITION_RESET);
    int z;
    for(z = 1;
        z < Game::n - 1 &&
        (game->cubes[Game::m][Game::m][z] != NULL ||
         game->cubes[Game::m][Game::m][z + 1] != NULL);
        z++);
    if(z == Game::n - 1){
        game->Finish();
        return;
    }
    this->x = this->y = Game::m;
    this->z = z;

}

void Player::StartMove(Player::Move move){

    if(game->Evolving() || game->finished) return;

    if(move == Player::FALL){
        if(game->cubes[this->x][this->y][this->z - 1] != NULL) return;
        this->simple_move = Player::TRANSLATE_DOWN;
        this->fall_channel = cfg->effect_player->Play(Game::PLAYER_FALLING);
    }

    else if(move == Player::TURN_RIGHT)
        this->simple_move = Player::ROTATE_RIGHT;

    else if(move == Player::TURN_LEFT)
        this->simple_move = Player::ROTATE_LEFT;

    else if(move == Player::STEP_FRONT || move == Player::STEP_BACK){
        int x, y, z;
        this->CalculateFrontBackPosition(
            &x, &y, &z,
            move == Player::STEP_FRONT ? 1 : -1
        );
        if(!Game::Inside(x, y, z) || game->cubes[x][y][z + 1] != NULL) return;
        if(game->cubes[x][y][z] == NULL)
            this->simple_move = (move == Player::STEP_FRONT ?
                                 Player::TRANSLATE_FRONT :
                                 Player::TRANSLATE_BACK);
        else if(z + 2 < Game::n &&
                game->cubes[x][y][z + 2] == NULL &&
                game->cubes[this->x][this->y][this->z + 2] == NULL)
            this->simple_move = Player::TRANSLATE_UP;
        else return;
    }

    else return;

    this->move = move;
    this->simple_move_part = 0.0;
    this->simple_move_start = ElapsedTime();
    ActivateAnimation();

}

void Player::ReleaseChannel(int channel){

    if(channel == this->fall_channel) this->fall_channel = -1;

}

void Player::HandleCollision(){

    if(game->cubes[this->x][this->y][this->z + 1] == NULL) return;
    delete game->cubes[this->x][this->y][this->z + 1];
    game->cubes[this->x][this->y][this->z + 1] = NULL;
    game->UpdateScore(Game::COLLISION);
    cfg->effect_player->Play(Game::COLLISION);

}

bool Player::Moving(){

    return this->move != Player::NO_MOVE;

}

void Player::StopMove(){

    if(!this->Moving()) return;
    this->move = Player::NO_MOVE;
    cfg->effect_player->Stop(this->fall_channel);
    this->fall_channel = -1;

}

bool Player::ContinueMove(){

    if(!this->Moving()) return false;
    cfg->display->PostRedisplay();
    int const time = ElapsedTime();
    int time_diff = time - this->simple_move_start;
    if(time_diff < 0){
        time_diff += INT_MAX;
        time_diff++;
    }
    this->simple_move_part = (time_diff /
                              (this->simple_move < Player::TRANSLATE_UP ?
                               250.0 :
                               200.0));
    if(this->simple_move_part < 1.0) return true;

    if(this->move == Player::FALL){
        this->z--;
        if(this->z > 0 && game->cubes[this->x][this->y][this->z - 1] == NULL){
            this->simple_move_part = 0.0;
            this->simple_move_start = time;
        }
        else{
            this->move = Player::NO_MOVE;
            cfg->effect_player->Stop(this->fall_channel, 500);
            this->fall_channel = -1;
            if(this->z == 0) this->ResetPosition();
        }
        game->UpdateScore(Game::PLAYER_FALLING);
    }

    else if(this->move == Player::TURN_RIGHT || this->move == Player::TURN_LEFT){
        this->orientation = (Player::Orientation)(
            (this->orientation + (this->move == Player::TURN_RIGHT ? 1 : 3)) % 4
        );
        this->move = Player::NO_MOVE;
    }

    else if(this->move == Player::STEP_FRONT || this->move == Player::STEP_BACK)
        if(this->simple_move == Player::TRANSLATE_UP){
            this->z++;
            this->simple_move = (this->move == Player::STEP_FRONT ?
                                 Player::TRANSLATE_FRONT :
                                 Player::TRANSLATE_BACK);
            this->simple_move_part = 0.0;
            this->simple_move_start = time;
        }
        else{
            bool finished = true;
            if(this->simple_move == Player::TRANSLATE_DOWN) this->z--;
            else{
                this->CalculateFrontBackPosition(
                    &this->x, &this->y, &this->z,
                    this->move == Player::STEP_FRONT ? 1 : -1
                );
                if(game->cubes[this->x][this->y][this->z - 1] == NULL){
                    finished = false;
                    this->simple_move = Player::TRANSLATE_DOWN;
                    this->simple_move_part = 0.0;
                    this->simple_move_start = time;
                }
            }
            if(finished){
                if(this->z == 0) this->ResetPosition();
                else if(game->cubes[this->x][this->y][this->z - 1] == NULL){
                    this->move = Player::FALL;
                    this->simple_move_part = 0.0;
                    this->simple_move_start = time;
                    this->fall_channel = cfg->effect_player->Play(Game::PLAYER_FALLING);
                }
                if(this->move != Player::FALL) this->move = Player::NO_MOVE;
                game->UpdateLights();
            }
        }

    return this->Moving();

}

void Player::Fall(){

    this->StartMove(Player::FALL);

}

void Player::TurnRight(){

    this->StartMove(Player::TURN_RIGHT);

}

void Player::TurnLeft(){

    this->StartMove(Player::TURN_LEFT);

}

void Player::StepFront(){

    this->StartMove(Player::STEP_FRONT);

}

void Player::StepBack(){

    this->StartMove(Player::STEP_BACK);

}

void Player::CreateLamp(){

    if(game->Evolving() || game->finished ||
       game->lamp_num == Lamp::num_max || this->lamps == 0)
        return;
    int x, y, z;
    this->CalculateFrontBackPosition(&x, &y, &z, 1);
    if(!Game::Inside(x, y, z)) return;
    this->lamps--;
    int const i = (game->lamp_first + game->lamp_num) % Lamp::num_max;
    game->lamps[i] = new Lamp(x, y, z, i);
    game->lamp_num++;
    game->RefreshLight(4 * Game::m);
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();

}

void Player::ObtainCube(){

    if(game->Evolving() || game->finished ||
       !game->cubes[this->x][this->y][this->z - 1]->DecreaseStock())
        return;
    this->cubes++;
    game->UpdateScore(Game::CUBE_OBTAINED);
    cfg->effect_player->Play(Game::CUBE_OBTAINED);
    cfg->display->PostRedisplay();

}

void Player::CreateCube(){

    if(game->Evolving() || game->finished ||
       this->cubes == 0)
        return;
    int x, y, z;
    this->CalculateFrontBackPosition(&x, &y, &z, 1);
    if(!Game::Inside(x, y, z)) return;
    if(game->cubes[x][y][z] != NULL){
        z++;
        if(game->cubes[x][y][z] != NULL) return;
    }
    this->cubes--;
    game->cubes[x][y][z] = new Cube(x, y, z, game->CreateStock());
    game->UpdateScore(Game::CUBE_CREATED);
    cfg->effect_player->Play(Game::CUBE_CREATED);
    cfg->display->PostRedisplay();

}

void Player::PushCube(){

    if(game->Evolving() || game->finished) return;
    int x, y, z, l, i;
    Cube *c, *c_next;
    this->CalculateFrontBackPosition(&x, &y, &z, 1);
    l = 1;
    z += l;
    if(Game::Inside(x, y, z)){
        if(game->cubes[x][y][z] == NULL){
            l--;
            z--;
            if(game->cubes[x][y][z] == NULL) return;
        }
        c_next = game->cubes[x][y][z];
        game->cubes[x][y][z] = NULL;
    }
    else c_next = NULL;
    for(c = c_next, i = 2; c != NULL; c = c_next, i++){
        this->CalculateFrontBackPosition(&x, &y, &z, i);
        z += l;
        if(Game::Inside(x, y, z)){
           c_next = game->cubes[x][y][z];
           game->cubes[x][y][z] = c;
           c->Move(x, y, z);
        }
        else{
            c_next = NULL;
            delete c;
        }
    }
    if(i == 2) return;
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();

}

void Player::DestroyCube(){

    if(game->Evolving() || game->finished) return;
    int x, y, z;
    this->CalculateFrontBackPosition(&x, &y, &z, 1);
    z++;
    if(!Game::Inside(x, y, z)) return;
    if(game->cubes[x][y][z] == NULL){
        z--;
        if(game->cubes[x][y][z] == NULL) return;
    }
    delete game->cubes[x][y][z];
    game->cubes[x][y][z] = NULL;
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();

}

void Player::DestroyColumn(){

    if(game->Evolving() || game->finished) return;
    int x, y, z;
    this->CalculateFrontBackPosition(&x, &y, &z, 1);
    if(!Game::Inside(x, y, z)) return;
    bool cube_destroyed = false, lamp_obtained = false;
    for(z = x != Game::m || y != Game::m ? 0 : 1; z < Game::n; z++){
        if(game->cubes[x][y][z] == NULL) continue;
        delete game->cubes[x][y][z];
        game->cubes[x][y][z] = NULL;
        if(!cube_destroyed) cube_destroyed = true;
        if(z <= this->z + 1) continue;
        game->UpdateScore(Game::COLUMN_DESTROYED);
        if(!lamp_obtained) lamp_obtained = true;
    }
    if(!cube_destroyed) return;
    if(lamp_obtained) this->lamps++;
    cfg->effect_player->Play(lamp_obtained ?
                             Game::COLUMN_DESTROYED :
                             Game::NEUTRAL);
    cfg->display->PostRedisplay();

}

void Player::VisualizeGridLines(bool full){

    Game::VisualizeGridLines(this->x, this->y, this->z, 1, 1, 2, false, full);

}

void Player::VisualizeGridPlanes(bool full){

    Game::VisualizeGridPlanes(this->x, this->y, this->z, 1, 1, 2, false, full);

}

void Player::VisualizeInfo(){

    static char const *const str[] = {"CUBES ", "LAMPS "};
    int num[] = {this->cubes, this->lamps};
    int length_max = 1;

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(5.0, 5.0, 0.0);
    for(int i = 0; i < 2; i++){
        glRasterPos2i(0, i * 15);
        glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *)str[i]);
        int length = 1;
        int j = 1;
        for(j *= 10; num[i] / j > 0; j *= 10) length++;
        if(length > length_max) length_max = length;
        for(j /= 10; j > 0; j /= 10){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0' + num[i] / j);
            num[i] %= j;
        }
    }
    glPopMatrix();

    glEnable(GL_BLEND);
    glColor4f(0.25, 0.25, 0.25, 0.5);
    glRecti(0, 0, 9 + 9 * (6 + length_max), 35);
    glDisable(GL_BLEND);

}

void Player::Visualize(){

    glPushMatrix();

    glTranslatef(this->x + 0.5, this->z, -(this->y + 0.5));

    if(this->orientation == Player::X_POS) glRotatef(-90.0, 0.0, 1.0, 0.0);
    else if(this->orientation == Player::Y_NEG) glRotatef(-180.0, 0.0, 1.0, 0.0);
    else if(this->orientation == Player::X_NEG) glRotatef(90.0, 0.0, 1.0, 0.0);

    if(this->Moving())
        if(this->simple_move == Player::ROTATE_RIGHT)
            glRotatef(-this->simple_move_part * 90.0, 0.0, 1.0, 0.0);
        else if(this->simple_move == Player::ROTATE_LEFT)
            glRotatef(this->simple_move_part * 90.0, 0.0, 1.0, 0.0);
        else if(this->simple_move == Player::TRANSLATE_UP)
            glTranslatef(0.0, this->simple_move_part, 0.0);
        else if(this->simple_move == Player::TRANSLATE_DOWN)
            glTranslatef(0.0, -this->simple_move_part, 0.0);
        else if(this->simple_move == Player::TRANSLATE_FRONT)
            glTranslatef(0.0, 0.0, -this->simple_move_part);
        else if(this->simple_move == Player::TRANSLATE_BACK)
            glTranslatef(0.0, 0.0, this->simple_move_part);

    Player::VisualizeFigure();

    glPopMatrix();

}





int Game::start_timers, Game::collapse_timers;
float Game::a, Game::d, Game::r, Game::w[5];
int Game::m, Game::n;

void Game::Initialize(time_t time, int argc, char **argv){

    srand(time);
    Player::Initialize(localtime(&time)->tm_yday == 0);
    Game::start_timers = Game::collapse_timers = 0;
    float x;
    int i, j;
    Game::a = 1.0;
    Game::d = 0.15;
    Game::r = 1.0;
    Game::m = 5;
    for(i = 1; i < argc; i++){
        if(strlen(argv[i]) < 3 || argv[i][1] != '=') continue;
        if(argv[i][0] == 'a' && (x = atof(argv[i] + 2)) > 0.0) Game::a = x;
        else if(argv[i][0] == 'd' && (x = atof(argv[i] + 2)) >= 0.0) Game::d = x;
        else if(argv[i][0] == 'r' && (x = atof(argv[i] + 2)) >= 0.0) Game::r = x;
        else if(argv[i][0] == 'm' && (j = atoi(argv[i] + 2)) >= 1) Game::m = j;
        else if(argv[i][0] == 's'){
            char fn[FILENAME_MAX + 1];
            FILE *const f = fopen(strcat(strcpy(fn,
                                                strchr(argv[i] + 2, '\\') == argv[i] + 2 ||
                                                strchr(argv[i] + 2, ':') != NULL ?
                                                "" :
                                                storage_folder),
                                         argv[i] + 2),
                                  "r");
            if(f != NULL){
                fscanf(f, "%f%f%f%d", &Game::a, &Game::d, &Game::r, &Game::m);
                fclose(f);
            }
        }
    }
    Game::w[0] = Game::a;
    for(i = 1, x = Game::d; i < 5; i++, x *= Game::r)
        if((Game::w[i] = Game::w[i - 1] - x) < 0.0) break;
    for(; i < 5; i++) Game::w[i] = 0.0;
    Game::n = 2 * Game::m + 1;

}

bool Game::Inside(int x, int y, int z){

    return (x >= 0 && x < Game::n &&
            y >= 0 && y < Game::n &&
            z >= 0 && z < Game::n);

}

void Game::VisualizeGridLines(int x_start, int y_start, int z_start,
                              int x_size, int y_size, int z_size,
                              bool inside, bool full){

    int const x_end = x_start + x_size;
    int const y_end = y_start + y_size;
    int const z_end = z_start + z_size;
    int const x_step = inside ? 1 : x_size;
    int const y_step = inside ? 1 : y_size;
    int const z_step = inside ? 1 : z_size;

    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    int x, y, z;
    glColor4f(0.0, 0.5, 0.0, 0.5);
    glBegin(GL_LINES);
    if(full){
        for(x = x_start; x <= x_end; x += x_step)
            for(y = y_start; y <= y_end; y += y_step){
                glVertex3f(x, 0.0, -y);
                glVertex3f(x, Game::n, -y);
            }
        for(y = y_start; y <= y_end; y += y_step)
            for(z = z_start; z <= z_end; z += z_step){
                glVertex3f(0.0, z, -y);
                glVertex3f(Game::n, z, -y);
            }
        for(z = z_start; z <= z_end; z += z_step)
            for(x = x_start; x <= x_end; x += x_step){
                glVertex3f(x, z, 0.0);
                glVertex3f(x, z, -Game::n);
            }
    }
    else{
        for(x = x_start; x <= x_end; x += x_step)
            for(y = y_start; y <= y_end; y += y_step){
                glVertex3f(x, z_start, -y);
                glVertex3f(x, z_end, -y);
            }
        for(y = y_start; y <= y_end; y += y_step)
            for(z = z_start; z <= z_end; z += z_step){
                glVertex3f(x_start, z, -y);
                glVertex3f(x_end, z, -y);
            }
        for(z = z_start; z <= z_end; z += z_step)
            for(x = x_start; x <= x_end; x += x_step){
                glVertex3f(x, z, -y_start);
                glVertex3f(x, z, -y_end);
            }
    }
    glEnd();

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

}

void Game::VisualizeGridPlanes(int x_start, int y_start, int z_start,
                               int x_size, int y_size, int z_size,
                               bool inside, bool full){

    static float const dev = 0.001;
    int const x_end = x_start + x_size;
    int const y_end = y_start + y_size;
    int const z_end = z_start + z_size;
    int const x_step = inside ? 1 : x_size;
    int const y_step = inside ? 1 : y_size;
    int const z_step = inside ? 1 : z_size;

    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);

    int x, y, z;
    glColor4f(0.0, 0.5, 0.0, 0.05);
    if(full){
        glBegin(GL_QUADS);
        for(x = x_start; x <= x_end; x += x_step){
            glVertex3f(x - dev, 0.0, 0.0);
            glVertex3f(x - dev, Game::n, 0.0);
            glVertex3f(x - dev, Game::n, -Game::n);
            glVertex3f(x - dev, 0.0, -Game::n);
            glVertex3f(x + dev, 0.0, 0.0);
            glVertex3f(x + dev, 0.0, -Game::n);
            glVertex3f(x + dev, Game::n, -Game::n);
            glVertex3f(x + dev, Game::n, 0.0);
        }
        for(y = y_start; y <= y_end; y += y_step){
            glVertex3f(0.0, 0.0, -(y - dev));
            glVertex3f(Game::n, 0.0, -(y - dev));
            glVertex3f(Game::n, Game::n, -(y - dev));
            glVertex3f(0.0, Game::n, -(y - dev));
            glVertex3f(0.0, 0.0, -(y + dev));
            glVertex3f(0.0, Game::n, -(y + dev));
            glVertex3f(Game::n, Game::n, -(y + dev));
            glVertex3f(Game::n, 0.0, -(y + dev));
        }
        for(z = z_start; z <= z_end; z += z_step){
            glVertex3f(0.0, z - dev, 0.0);
            glVertex3f(0.0, z - dev, -Game::n);
            glVertex3f(Game::n, z - dev, -Game::n);
            glVertex3f(Game::n, z - dev, 0.0);
            glVertex3f(0.0, z + dev, 0.0);
            glVertex3f(Game::n, z + dev, 0.0);
            glVertex3f(Game::n, z + dev, -Game::n);
            glVertex3f(0.0, z + dev, -Game::n);
        }
        glEnd();
    }
    else{
        for(x = x_start; x <= x_end; x += x_step){
            glBegin(GL_POLYGON);
            glVertex3f(x - dev, z_start, -y_start);
            glVertex3f(x - dev, z_start, 0.0);
            glVertex3f(x - dev, z_end, 0.0);
            glVertex3f(x - dev, z_end, -y_start);
            glVertex3f(x - dev, Game::n, -y_start);
            glVertex3f(x - dev, Game::n, -y_end);
            glVertex3f(x - dev, z_end, -y_end);
            glVertex3f(x - dev, z_end, -Game::n);
            glVertex3f(x - dev, z_start, -Game::n);
            glVertex3f(x - dev, z_start, -y_end);
            glVertex3f(x - dev, 0.0, -y_end);
            glVertex3f(x - dev, 0.0, -y_start);
            glEnd();
            glBegin(GL_POLYGON);
            glVertex3f(x + dev, z_start, -y_start);
            glVertex3f(x + dev, 0.0, -y_start);
            glVertex3f(x + dev, 0.0, -y_end);
            glVertex3f(x + dev, z_start, -y_end);
            glVertex3f(x + dev, z_start, -Game::n);
            glVertex3f(x + dev, z_end, -Game::n);
            glVertex3f(x + dev, z_end, -y_end);
            glVertex3f(x + dev, Game::n, -y_end);
            glVertex3f(x + dev, Game::n, -y_start);
            glVertex3f(x + dev, z_end, -y_start);
            glVertex3f(x + dev, z_end, 0.0);
            glVertex3f(x + dev, z_start, 0.0);
            glEnd();
        }
        for(y = y_start; y <= y_end; y += y_step){
            glBegin(GL_POLYGON);
            glVertex3f(x_start, z_start, -(y - dev));
            glVertex3f(x_start, 0.0, -(y - dev));
            glVertex3f(x_end, 0.0, -(y - dev));
            glVertex3f(x_end, z_start, -(y - dev));
            glVertex3f(Game::n, z_start, -(y - dev));
            glVertex3f(Game::n, z_end, -(y - dev));
            glVertex3f(x_end, z_end, -(y - dev));
            glVertex3f(x_end, Game::n, -(y - dev));
            glVertex3f(x_start, Game::n, -(y - dev));
            glVertex3f(x_start, z_end, -(y - dev));
            glVertex3f(0.0, z_end, -(y - dev));
            glVertex3f(0.0, z_start, -(y - dev));
            glEnd();
            glBegin(GL_POLYGON);
            glVertex3f(x_start, z_start, -(y + dev));
            glVertex3f(0.0, z_start, -(y + dev));
            glVertex3f(0.0, z_end, -(y + dev));
            glVertex3f(x_start, z_end, -(y + dev));
            glVertex3f(x_start, Game::n, -(y + dev));
            glVertex3f(x_end, Game::n, -(y + dev));
            glVertex3f(x_end, z_end, -(y + dev));
            glVertex3f(Game::n, z_end, -(y + dev));
            glVertex3f(Game::n, z_start, -(y + dev));
            glVertex3f(x_end, z_start, -(y + dev));
            glVertex3f(x_end, 0.0, -(y + dev));
            glVertex3f(x_start, 0.0, -(y + dev));
            glEnd();
        }
        for(z = z_start; z <= z_end; z += z_step){
            glBegin(GL_POLYGON);
            glVertex3f(x_start, z - dev, -y_start);
            glVertex3f(0.0, z - dev, -y_start);
            glVertex3f(0.0, z - dev, -y_end);
            glVertex3f(x_start, z - dev, -y_end);
            glVertex3f(x_start, z - dev, -Game::n);
            glVertex3f(x_end, z - dev, -Game::n);
            glVertex3f(x_end, z - dev, -y_end);
            glVertex3f(Game::n, z - dev, -y_end);
            glVertex3f(Game::n, z - dev, -y_start);
            glVertex3f(x_end, z - dev, -y_start);
            glVertex3f(x_end, z - dev, 0.0);
            glVertex3f(x_start, z - dev, 0.0);
            glEnd();
            glBegin(GL_POLYGON);
            glVertex3f(x_start, z + dev, -y_start);
            glVertex3f(x_start, z + dev, 0.0);
            glVertex3f(x_end, z + dev, 0.0);
            glVertex3f(x_end, z + dev, -y_start);
            glVertex3f(Game::n, z + dev, -y_start);
            glVertex3f(Game::n, z + dev, -y_end);
            glVertex3f(x_end, z + dev, -y_end);
            glVertex3f(x_end, z + dev, -Game::n);
            glVertex3f(x_start, z + dev, -Game::n);
            glVertex3f(x_start, z + dev, -y_end);
            glVertex3f(0.0, z + dev, -y_end);
            glVertex3f(0.0, z + dev, -y_start);
            glEnd();
        }
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

}

Game::Game(){

    this->light = -1;
    this->score = 0;
    this->stage = 0;

    double sum = 0.0;
    for(int i = 0; i < 5; i++)
        this->stock_cum_prob[i] = sum += this->stock_weight[i] = 1.0;
    for(int i = 0; i < 5; i++)
        this->stock_cum_prob[i] /= sum;

    this->grid_lines = this->grid_planes = Game::INVISIBLE;

    this->sun = NULL;

    this->lamp_first = this->lamp_num = 0;

    this->cubes_collapsing = false;
    this->collapse_map = new bool *[Game::n];
    this->cubes = new Cube ***[Game::n];
    for(int x = 0; x < Game::n; x++){
        this->collapse_map[x] = new bool[Game::n];
        this->cubes[x] = new Cube **[Game::n];
        for(int y = 0; y < Game::n; y++){
            this->cubes[x][y] = new Cube *[Game::n];
            for(int z = 0; z < Game::n; z++) this->cubes[x][y][z] = NULL;
        }
    }

    this->still_cameras[0] = new StillCamera(60.0, 0, 0, Game::n);
    this->still_cameras[1] = new StillCamera(60.0, 0, Game::n, Game::n);
    this->still_cameras[2] = new StillCamera(60.0, Game::n, Game::n, Game::n);
    this->still_cameras[3] = new StillCamera(60.0, Game::n, 0, Game::n);
    this->still_camera = 0;

    this->player = NULL;

    this->camera = this->still_cameras[this->still_camera];

    this->finished = false;

}

Game::Game(FILE *f){

    fscanf(f, "%d", &this->light);
    fgetc(f);
    fscanf(f, "%d", &this->score);
    fgetc(f);
    fscanf(f, "%d", &this->stage);
    fgetc(f);

    for(int i = 0; i < 5; i++){
        fscanf(f, "%lf", this->stock_weight + i);
        fgetc(f);
    }
    for(int i = 0; i < 5; i++){
        fscanf(f, "%lf", this->stock_cum_prob + i);
        fgetc(f);
    }

    fscanf(f, "%d", &this->grid_lines);
    fgetc(f);
    fscanf(f, "%d", &this->grid_planes);
    fgetc(f);

    char c;

    if((c = fgetc(f)) == '\n') this->sun = NULL;
    else{
        ungetc(c, f);
        this->sun = new Sun(f);
    }

    this->lamp_first = 0;
    fscanf(f, "%d", &this->lamp_num);
    fgetc(f);
    for(int i = 0; i < this->lamp_num; i++) this->lamps[i] = new Lamp(f, i);

    this->cubes_collapsing = false;
    this->collapse_map = new bool *[Game::n];
    this->cubes = new Cube ***[Game::n];
    for(int x = 0; x < Game::n; x++){
        this->collapse_map[x] = new bool[Game::n];
        this->cubes[x] = new Cube **[Game::n];
        for(int y = 0; y < Game::n; y++){
            this->cubes[x][y] = new Cube *[Game::n];
            for(int z = 0; z < Game::n; z++)
                if((c = fgetc(f)) == '\n') this->cubes[x][y][z] = NULL;
                else{
                    ungetc(c, f);
                    this->cubes[x][y][z] = new Cube(x, y, z, f);
                }
        }
    }

    for(int i = 0; i < 4; i++) this->still_cameras[i] = new StillCamera(f);
    fscanf(f, "%d", &this->still_camera);
    fgetc(f);

    this->player = new Player(f);

    this->camera = ((fgetc(f) - '0') ?
                    (Camera *)this->player->camera :
                    (Camera *)this->still_cameras[this->still_camera]);
    fgetc(f);

    this->finished = fgetc(f) - '0';
    fgetc(f);

}

Game::~Game(){

    delete this->sun;

    for(int i = 0; i < this->lamp_num; i++)
        delete this->lamps[(this->lamp_first + i) % Lamp::num_max];

    for(int x = 0; x < Game::n; x++){
        for(int y = 0; y < Game::n; y++){
            for(int z = 0; z < Game::n; z++) delete this->cubes[x][y][z];
            delete[] this->cubes[x][y];
        }
        delete[] this->collapse_map[x];
        delete[] this->cubes[x];
    }
    delete[] this->collapse_map;
    delete[] this->cubes;

    for(int i = 0; i < 4; i++) delete this->still_cameras[i];

    delete this->player;

}

void Game::Store(FILE *f){

    fprintf(f, "%d\n", this->light);
    fprintf(f, "%d\n", this->score);
    fprintf(f, "%d\n", this->stage);

    for(int i = 0; i < 5; i++)
        fprintf(f, "%.10f\n", this->stock_weight[i]);
    for(int i = 0; i < 5; i++)
        fprintf(f, "%.10f\n", this->stock_cum_prob[i]);

    fprintf(f, "%d\n", this->grid_lines);
    fprintf(f, "%d\n", this->grid_planes);

    if(this->sun == NULL) fprintf(f, "\n");
    else this->sun->Store(f);

    fprintf(f, "%d\n", this->lamp_num);
    for(int i = 0; i < this->lamp_num; i++)
        this->lamps[(this->lamp_first + i) % Lamp::num_max]->Store(f);

    for(int x = 0; x < Game::n; x++)
        for(int y = 0; y < Game::n; y++)
            for(int z = 0; z < Game::n; z++)
                if(this->cubes[x][y][z] == NULL) fprintf(f, "\n");
                else this->cubes[x][y][z]->Store(f);

    for(int i = 0; i < 4; i++) this->still_cameras[i]->Store(f);
    fprintf(f, "%d\n", this->still_camera);

    this->player->Store(f);

    fprintf(f, "%d\n", this->camera == this->player->camera);

    fprintf(f, "%d\n", this->finished);

}

bool Game::ContinueStart0(){

    if(!this->Starting()) return false;
    for(int x = 0; x < Game::n; x++)
        for(int y = 0; y < Game::n; y++)
            this->cubes[x][y][0] = new Cube(x, y, 0, this->CreateStock());
    delete this->cubes[Game::m][Game::m][0];
    this->cubes[Game::m][Game::m][0] = new Cube(Game::m, Game::m, 0, 0);
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();
    return true;

}

bool Game::ContinueStart1(){

    if(!this->Starting()) return false;
    this->player = new Player(Game::m, Game::m, 1, Player::Y_POS);
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();
    return true;

}

bool Game::ContinueStart2(){

    if(!this->Starting()) return false;
    this->camera = this->player->camera;
    this->UpdateScore(Game::START);
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();
    return true;

}

bool Game::Starting(){

    return this->stage == 0;

}

bool Game::Evolving(){

    return this->Starting() || this->cubes_collapsing || this->player->Moving();

}

void Game::Finish(){

    if(this->finished) return;
    this->finished = true;
    this->cubes_collapsing = false;
    this->player->StopMove();
    cfg->music_player->Play();

}

void Game::UpdateScore(Game::Event event){

    if(event == Game::START) this->score += 100;
    else if(event == Game::POSITION_RESET) this->score -= 100;
    else if(event == Game::COLLISION) this->score -= 100;
    else if(event == Game::CUBE_CREATED) this->score += 10;
    else if(event == Game::CUBE_OBTAINED) this->score -= 10;
    else if(event == Game::COLUMN_DESTROYED) this->score += 10;
    else if(event == Game::PLAYER_FALLING) this->score -= 10;
    else if(event == Game::CUBES_COLLAPSING) this->score += 1;

    if(this->score > this->stage * 1000){
        this->stage++;
        double sum = 0.0;
        for(int i = 0; i < 5; i++)
            this->stock_cum_prob[i] = sum += this->stock_weight[i] *= Game::w[i];
        for(int i = 0; i < 5; i++)
            this->stock_cum_prob[i] /= sum;
        delete this->sun;
        this->sun = new Sun();
        this->light = 5 * Game::n;
        cfg->music_player->Play();
    }
    else if(this->score < 0) this->Finish();

}

void Game::UpdateLights(){

    while(this->lamp_num > 0 && !this->lamps[this->lamp_first]->Weaken()){
        this->lamp_first = (this->lamp_first + 1) % Lamp::num_max;
        this->lamp_num--;
    }
    for(int i = 1; i < this->lamp_num; i++)
        this->lamps[(this->lamp_first + i) % Lamp::num_max]->Weaken();

    if(this->sun != NULL && !this->sun->Move()){
        this->sun = NULL;
        if(this->lamp_num > 0) cfg->music_player->Play();
    }

    this->light--;
    if(this->light < 0) this->Finish();

}

void Game::RefreshLight(int light){

    if(this->light < light) this->light = light;

}

int Game::CreateStock(){

    double const p = rand() / (RAND_MAX + 1.0);
    int i;
    for(i = 0; this->stock_cum_prob[i] <= p; i++);
    return i;

}

void Game::StartCollapse(){

    if(this->Evolving() || this->finished) return;
    this->cubes_collapsing = true;
    for(int x = 0; x < Game::n; x++)
        for(int y = 0; y < Game::n; y++)
            this->collapse_map[x][y] = this->cubes[x][y][0] == NULL;
    SetTimer(CollapseTimerFunction);
    Game::collapse_timers++;

}

bool Game::ContinueCollapse(){

    if(!this->cubes_collapsing) return false;
    this->cubes_collapsing = false;
    for(int x = 0; x < Game::n; x++)
        for(int y = 0; y < Game::n; y++){
            if(this->cubes[x][y][0] != NULL &&
               this->collapse_map[x][y]){
                if(!this->cubes_collapsing) this->cubes_collapsing = true;
                delete this->cubes[x][y][0];
                this->cubes[x][y][0] = NULL;
                this->UpdateScore(Game::CUBES_COLLAPSING);
            }
            for(int z = 1; z < Game::n; z++)
                if(this->cubes[x][y][z] != NULL &&
                   this->cubes[x][y][z - 1] == NULL){
                    if(!this->cubes_collapsing) this->cubes_collapsing = true;
                    this->cubes[x][y][z - 1] = this->cubes[x][y][z];
                    this->cubes[x][y][z]->Move(x, y, z - 1);
                    this->cubes[x][y][z] = NULL;
                    this->UpdateScore(Game::CUBES_COLLAPSING);
                }
        }
    this->player->Fall();
    if(!this->cubes_collapsing) return false;
    this->player->HandleCollision();
    cfg->effect_player->Play(Game::CUBES_COLLAPSING);
    cfg->display->PostRedisplay();
    return true;

}

void Game::UseChangeStillCamera(){

    if(this->Starting() || this->finished) return;
    if(this->camera != this->player->camera)
        this->still_camera = (this->still_camera + 1) % 4;
    this->camera = this->still_cameras[this->still_camera];
    cfg->display->PostRedisplay();

}

void Game::UsePlayerCamera(){

    if(this->Starting() || this->finished ||
       this->camera == this->player->camera)
        return;
    this->camera = this->player->camera;
    cfg->display->PostRedisplay();

}

void Game::ChangeGridLines(){

    if(this->Starting() || this->finished) return;
    this->grid_lines = (Game::GridVisibility)((this->grid_lines + 1) % 4);
    cfg->display->PostRedisplay();

}

void Game::ChangeGridPlanes(){

    if(this->Starting() || this->finished) return;
    this->grid_planes = (Game::GridVisibility)((this->grid_planes + 1) % 4);
    cfg->display->PostRedisplay();

}

void Game::VisualizeInfo(){

    static char const *const str[] = {"STAGE ", "SCORE ", "LIGHT "};
    int num[] = {this->stage, this->score, this->light};
    int length_max = 1;

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(5.0, 5.0, 0.0);
    for(int i = 0; i < 3; i++){
        glRasterPos2i(0, i * 15);
        glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *)str[i]);
        int length = 1;
        if(num[i] < 0){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '-');
            num[i] *= -1;
            length++;
        }
        int j = 1;
        for(j *= 10; num[i] / j > 0; j *= 10) length++;
        if(length > length_max) length_max = length;
        for(j /= 10; j > 0; j /= 10){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0' + num[i] / j);
            num[i] %= j;
        }
    }
    glPopMatrix();

    glEnable(GL_BLEND);
    glColor4f(0.25, 0.25, 0.25, 0.5);
    glRecti(0, 0, 9 + 9 * (6 + length_max), 50);
    glDisable(GL_BLEND);

}

void Game::VisualizeWalls(){

    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    bool bright = false;
    int x, y, z;
    //back
    glNormal3f(0.0, 0.0, -1.0);
    for(z = 0; z < Game::n; z++)
        for(x = 0; x < Game::n; x++, bright = !bright){
            if(this->cubes[x][0][z] != NULL) continue;
            glColor4f(bright ? 0.25 : 0.2, bright ? 0.25 : 0.2, 1.0, 0.95);
            glVertex3f(x, z, 0.0);
            glVertex3f(x, z + 1.0, 0.0);
            glVertex3f(x + 1.0, z + 1.0, 0.0);
            glVertex3f(x + 1.0, z, 0.0);
        }
    //left
    glNormal3f(1.0, 0.0, 0.0);
    for(z = 0; z < Game::n; z++)
        for(y = 0; y < Game::n; y++, bright = !bright){
            if(this->cubes[0][y][z] != NULL) continue;
            glColor4f(bright ? 0.25 : 0.2, bright ? 0.25 : 0.2, 1.0, 0.95);
            glVertex3f(0.0, z, -y);
            glVertex3f(0.0, z, -(y + 1.0));
            glVertex3f(0.0, z + 1.0, -(y + 1.0));
            glVertex3f(0.0, z + 1.0, -y);
        }
    //front
    glNormal3f(0.0, 0.0, 1.0);
    for(z = 0; z < Game::n; z++)
        for(x = 0; x < Game::n; x++, bright = !bright){
            if(this->cubes[x][Game::n - 1][z] != NULL) continue;
            glColor4f(bright ? 0.25 : 0.2, bright ? 0.25 : 0.2, 1.0, 0.95);
            glVertex3f(x, z, -Game::n);
            glVertex3f(x + 1.0, z, -Game::n);
            glVertex3f(x + 1.0, z + 1.0, -Game::n);
            glVertex3f(x, z + 1.0, -Game::n);
        }
    //right
    glNormal3f(-1.0, 0.0, 0.0);
    for(z = 0; z < Game::n; z++)
        for(y = 0; y < Game::n; y++, bright = !bright){
            if(this->cubes[Game::n - 1][y][z] != NULL) continue;
            glColor4f(bright ? 0.25 : 0.2, bright ? 0.25 : 0.2, 1.0, 0.95);
            glVertex3f(Game::n, z, -y);
            glVertex3f(Game::n, z + 1.0, -y);
            glVertex3f(Game::n, z + 1.0, -(y + 1.0));
            glVertex3f(Game::n, z, -(y + 1.0));
        }
    glEnd();
    glDisable(GL_BLEND);

}

void Game::Visualize(){

    if(!this->camera->Visualize()) return;

    float ambient[] = {0.0, 0.0, 0.0, 1.0};
    if(this->light < 0) ambient[0] = ambient[1] = ambient[2] = 0.2;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glDisable(GL_LIGHT0);
    if(this->sun != NULL) this->sun->Visualize();

    int i;
    for(i = 0; i < Lamp::num_max; i++) glDisable(GL_LIGHT1 + i);
    for(i = 0; i < this->lamp_num; i++)
        this->lamps[(this->lamp_first + i) % Lamp::num_max]->VisualizeLight();
    for(i--; i >= 0; i--)
        this->lamps[(this->lamp_first + i) % Lamp::num_max]->Visualize();

    for(int x = 0; x < Game::n; x++)
        for(int y = 0; y < Game::n; y++)
            for(int z = 0; z < Game::n; z++)
                if(this->cubes[x][y][z] != NULL) this->cubes[x][y][z]->Visualize();

    if(this->player != NULL) this->player->Visualize();

    this->VisualizeWalls();

    glDisable(GL_LIGHTING);
    if(this->grid_lines <= Game::VISIBLE_PLAYER_FULL)
        this->player->VisualizeGridLines(this->grid_lines == Game::VISIBLE_PLAYER_FULL);
    else if(this->grid_lines == Game::VISIBLE_ALL) Game::VisualizeGridLines();
    if(this->grid_planes <= Game::VISIBLE_PLAYER_FULL)
        this->player->VisualizeGridPlanes(this->grid_planes == Game::VISIBLE_PLAYER_FULL);
    else if(this->grid_planes == Game::VISIBLE_ALL) Game::VisualizeGridPlanes();
    glEnable(GL_LIGHTING);

}





int (*Display::SetSwapInterval)(int);
bool Display::avsync_possible;

void Display::Initialize(){

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window *const w = SDL_CreateWindow("", 0, 0, 0, 0,
                                           SDL_WINDOW_OPENGL |
                                           SDL_WINDOW_HIDDEN);
    SDL_GLContext const c = SDL_GL_CreateContext(w);

    char *(*const GetExtensionsString)() =
        (char *(*)())glutGetProcAddress("wglGetExtensionsStringEXT");
    if(GetExtensionsString != NULL)
        for(char *ext = strtok(GetExtensionsString(), " ");
            ext != NULL && !Display::avsync_possible;
            ext = strtok(NULL, " "))
            if(strcmp(ext, "WGL_EXT_swap_control_tear") == 0){
                if(Display::SetSwapInterval == NULL)
                    Display::SetSwapInterval =
                        (int (*)(int))glutGetProcAddress("wglSwapIntervalEXT");
                Display::avsync_possible = true;
            }
            else if(strcmp(ext, "WGL_EXT_swap_control") == 0)
                Display::SetSwapInterval =
                    (int (*)(int))glutGetProcAddress("wglSwapIntervalEXT");

    SDL_GL_DeleteContext(c);
    SDL_DestroyWindow(w);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

}

Display::Display(){

    this->rendering = Display::VSYNC;
    this->id = -1;
    this->showing = false;

}

Display::Display(FILE *f){

    fscanf(f, "%d", &this->rendering);
    fgetc(f);
    this->id = -1;
    this->showing = false;

}

void Display::Store(FILE *f){

    fprintf(f, "%d\n", this->rendering);

}

void Display::LocalizePointerPosition(int *x, int *y, int *x_, int *y_){}

void Display::GlobalizePointerPosition(int *x, int *y, int *x_, int *y_){}

bool Display::Show(int pointer_x, int pointer_y,
                   int pointer_x_, int pointer_y_){

    if(this->id != -1) return false;
    cfg->pointer->x = pointer_x;
    cfg->pointer->y = pointer_y;
    cfg->pointer->x_ = pointer_x_;
    cfg->pointer->y_ = pointer_y_;
    this->showing = true;
    glutInitDisplayMode(GLUT_RGBA |
                        (this->rendering == Display::SINGLE ?
                         GLUT_SINGLE :
                         GLUT_DOUBLE) |
                        GLUT_DEPTH |
                        GLUT_MULTISAMPLE);
    return true;

}

void Display::Show_(){

    glutKeyboardFunc(cfg->game_i_o == NULL ?
                     MainKeyboardFunction :
                     IOKeyboardFunction);
    glutSpecialFunc(cfg->game_i_o == NULL ?
                    MainSpecialFunction :
                    IOSpecialFunction);
    glutMouseFunc(MouseFunction);
    glutMouseWheelFunc(MouseWheelFunction);
    glutKeyboardUpFunc(NULL);
    glutSpecialUpFunc(NULL);
    glutMotionFunc(NULL);
    cfg->pointer->Show();

    if(this->rendering != Display::SINGLE &&
       Display::SetSwapInterval != NULL)
        Display::SetSwapInterval(this->rendering == Display::DOUBLE ?
                                 0 :
                                 this->rendering == Display::AVSYNC &&
                                 Display::avsync_possible ?
                                 -1 :
                                 1);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, this->width, this->height);

}

bool Display::Hide(){

    if(this->id == -1 || this->showing) return false;
    cfg->pointer->x = cfg->pointer->y = -1;
    cfg->pointer->x_ = cfg->pointer->y_ = 0;
    return true;

}

void Display::PostRedisplay(){

    if(this->id == -1) return;
    glutPostWindowRedisplay(this->id);

}

void Display::VisualizeDimLayer(){

    glEnable(GL_BLEND);
    glColor4f(0.0, 0.0, 0.0, 0.75);
    glRecti(0, 0, this->width, this->height);
    glDisable(GL_BLEND);

}

void Display::VisualizeFinished(){

    static char const *const str = "FINISHED";
    static int const str_height = glutBitmapHeight(GLUT_BITMAP_HELVETICA_18) - 9;
    static int const str_width = glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                                  (unsigned char *)str) - 2;

    glPushMatrix();
    glTranslatef((this->width - str_width) / 2,
                 (this->height - str_height) / 2,
                 0.0);

    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-1.0, 0.0, 0.0);
    glRasterPos2i(0, 0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)str);

    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 0.5);
    glTranslatef(-4.0, -5.0, 0.0);
    glRecti(0, 0, str_width + 10, str_height + 10);
    glDisable(GL_BLEND);

    glPopMatrix();

}





void Window::Initialize(){

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1, 1);

}

Window::Window() :
Display(){

    this->x = this->y = 0;
    this->width = glutGet(GLUT_SCREEN_WIDTH) * 15 / 16;
    this->height = glutGet(GLUT_SCREEN_HEIGHT) * 15 / 16;
    this->border_width = glutGet(GLUT_WINDOW_BORDER_WIDTH);
    this->border_height = glutGet(GLUT_WINDOW_BORDER_HEIGHT);
    this->width -= 2 * this->border_width;
    this->height -= this->border_width + this->border_height;
    int const width = this->height * 4 / 3;
    if(this->width > width) this->width = width;
    else if(this->width < width) this->height = this->width * 3 / 4;
    this->damaged = this->repair_scheduled = false;

}

Window::Window(FILE *f) :
Display(f){

    fscanf(f, "%d%d%d%d", &this->x, &this->y, &this->width, &this->height);
    fgetc(f);
    this->border_width = glutGet(GLUT_WINDOW_BORDER_WIDTH);
    this->border_height = glutGet(GLUT_WINDOW_BORDER_HEIGHT);
    this->damaged = this->repair_scheduled = false;

}

void Window::Store(FILE *f){

    this->Display::Store(f);
    fprintf(f, "%d %d %d %d\n", this->x, this->y, this->width, this->height);

}

void Window::LocalizePointerPosition(int *x, int *y, int *x_, int *y_){

    *x -= this->x + this->border_width;
    *y -= this->y + this->border_height;
    if(*x + *x_ < 0 || *x + *x_ >= this->width ||
       *y + *y_ < 0 || *y + *y_ >= this->height){
        *x = *y = -1;
        *x_ = *y_ = 0;
    }

}

void Window::GlobalizePointerPosition(int *x, int *y, int *x_, int *y_){

    if(*x == -1 || *y == -1) return;
    *x += this->x + this->border_width;
    *y += this->y + this->border_height;

}

bool Window::Show(int pointer_x, int pointer_y,
                  int pointer_x_, int pointer_y_){

    if(!this->Display::Show(pointer_x, pointer_y, pointer_x_, pointer_y_)) return false;
    this->id = glutCreateWindow("L in Cubeland");
    glutPositionWindow(this->x, this->y);
    glutReshapeWindow(this->width, this->height);
    glutDisplayFunc(WindowInitDisplayFunction);
    glutPositionFunc(InitPositionFunction);
    glutReshapeFunc(NULL);
    glutEntryFunc(EntryFunction);
    glutCloseFunc(NULL);
    glutWindowStatusFunc(NULL);
    this->Show_();
    return true;

}

bool Window::Hide(){

    if(this->repair_scheduled || !this->Display::Hide()) return false;
    glutDestroyWindow(this->id);
    this->id = -1;
    return true;

}

void Window::Repair(){

    int const x = cfg->pointer->x, y = cfg->pointer->y;
    int const x_ = cfg->pointer->x_, y_ = cfg->pointer->y_;
    this->Hide();
    this->Show(x, y, x_, y_);

}





FullScreen::FullScreen() :
Display(){

    this->width = glutGet(GLUT_SCREEN_WIDTH);
    this->height = glutGet(GLUT_SCREEN_HEIGHT);

}

FullScreen::FullScreen(FILE *f) :
Display(f){

    this->width = glutGet(GLUT_SCREEN_WIDTH);
    this->height = glutGet(GLUT_SCREEN_HEIGHT);

}

bool FullScreen::Show(int pointer_x, int pointer_y,
                      int pointer_x_, int pointer_y_){

    if(!this->Display::Show(pointer_x, pointer_y, pointer_x_, pointer_y_)) return false;
    this->id = glutEnterGameMode();
    glutDisplayFunc(InitDisplayFunction);
    glutPositionFunc(NULL);
    glutReshapeFunc(NULL);
    this->Show_();
    return true;

}

bool FullScreen::Hide(){

    if(!this->Display::Hide()) return false;
    glutLeaveGameMode();
    this->id = -1;
    return true;

}





Adjustment::Adjustment(){}

void Adjustment::Toggle(){}

void Adjustment::Reset(){}

void Adjustment::Increase(){}

void Adjustment::Decrease(){}





AutoHidable::AutoHidable(void (*TimerFunction)(int)){

    this->visible = false;
    this->timers = 0;
    this->TimerFunction = TimerFunction;

}

bool AutoHidable::MakeVisible(){

    if(this->visible) return false;
    this->visible = true;
    return true;

}

bool AutoHidable::MakeInvisible(){

    if(!this->visible) return false;
    this->visible = false;
    return true;

}

void AutoHidable::IncreaseTimers(){

    this->MakeVisible();
    SetTimer(this->TimerFunction, 2000);
    this->timers++;

}

void AutoHidable::DecreaseTimers(){

    this->timers--;
    if(this->timers == 0) this->MakeInvisible();

}





char const *const SoundPlayer::sound_folder = "..\\sound\\";

void SoundPlayer::Initialize(){

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);

}

void SoundPlayer::Terminate(){

    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

}

SoundPlayer::SoundPlayer(void (*TimerFunction)(int)) :
AutoHidable(TimerFunction){

    this->active = true;
    this->volume = 15;

}

SoundPlayer::SoundPlayer(FILE *f, void (*TimerFunction)(int)) :
AutoHidable(TimerFunction){

    this->active = fgetc(f) - '0';
    fgetc(f);
    fscanf(f, "%d", &this->volume);
    fgetc(f);

}

void SoundPlayer::Store(FILE *f){

    fprintf(f, "%d %d\n", this->active, this->volume);

}

int SoundPlayer::CalculateActualVolume(){

    return (pow(3.0, this->volume / 15.0) - 1.0) / 8.0 * MIX_MAX_VOLUME + 0.5;

}

void SoundPlayer::Toggle(){

    this->IncreaseTimers();
    this->ToggleActive();

}

void SoundPlayer::Reset(){

    this->IncreaseTimers();
    this->ResetVolume();

}

void SoundPlayer::Increase(){

    this->IncreaseTimers();
    this->IncreaseVolume();

}

void SoundPlayer::Decrease(){

    this->IncreaseTimers();
    this->DecreaseVolume();

}

bool SoundPlayer::Visible(){

    return this->visible;

}

bool SoundPlayer::MakeVisible(){

    if(!this->AutoHidable::MakeVisible()) return false;
    cfg->display->PostRedisplay();
    return true;

}

bool SoundPlayer::MakeInvisible(){

    if(!this->AutoHidable::MakeInvisible()) return false;
    cfg->display->PostRedisplay();
    return true;

}

void SoundPlayer::ToggleActive(){

    this->active = !this->active;
    if(this->visible) cfg->display->PostRedisplay();

}

bool SoundPlayer::ResetVolume(){

    if(this->volume == 15) return false;
    this->volume = 15;
    if(this->visible) cfg->display->PostRedisplay();
    return true;

}

bool SoundPlayer::IncreaseVolume(){

    if(this->volume == 30) return false;
    this->volume++;
    if(this->visible) cfg->display->PostRedisplay();
    return true;

}

bool SoundPlayer::DecreaseVolume(){

    if(this->volume == 0) return false;
    this->volume--;
    if(this->visible) cfg->display->PostRedisplay();
    return true;

}

void SoundPlayer::Visualize(){

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(5.0, 10.0, 0.0);
    glPushMatrix();
    glTranslatef(5.0, 0.0, 0.0);
    (this->active ? glutSolidCylinder : glutWireCylinder)(5.0, 0.0, 10, 1);
    glPopMatrix();
    glTranslatef(0.0, 10.0, 0.0);
    glRecti(0, 0, 1, 92);
    glRecti(9, 0, 10, 92);
    glRecti(1, 0, 9, 1 + 3 * this->volume);
    glRecti(1, 91, 9, 92);
    glPopMatrix();

    glEnable(GL_BLEND);
    glColor4f(0.25, 0.25, 0.25, 0.5);
    glRecti(0, 0, 20, 117);
    glDisable(GL_BLEND);

}





Mix_Music *MusicPlayer::day_time;
Mix_Music *MusicPlayer::night_time;
Mix_Music *MusicPlayer::finished;

void MusicPlayer::Initialize(bool new_year){

    char fn[FILENAME_MAX + 1];
    MusicPlayer::day_time = Mix_LoadMUS(
        strcat(strcpy(fn, SoundPlayer::sound_folder),
               new_year ? "0.mp3" : "day-time.mp3")
    );
    MusicPlayer::night_time = Mix_LoadMUS(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "night-time.mp3")
    );
    MusicPlayer::finished = Mix_LoadMUS(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "finished.mp3")
    );

}

void MusicPlayer::Terminate(){

    Mix_FreeMusic(MusicPlayer::day_time);
    Mix_FreeMusic(MusicPlayer::night_time);
    Mix_FreeMusic(MusicPlayer::finished);

}

MusicPlayer::MusicPlayer(void (*TimerFunction)(int)) :
SoundPlayer(TimerFunction){

    this->music = NULL;
    Mix_VolumeMusic(this->CalculateActualVolume());

}

MusicPlayer::MusicPlayer(FILE *f, void (*TimerFunction)(int)) :
SoundPlayer(f, TimerFunction){

    this->music = NULL;
    if(this->active) Mix_VolumeMusic(this->CalculateActualVolume());
    else Mix_PauseMusic();

}

MusicPlayer::~MusicPlayer(){

    Mix_HaltMusic();

}

void MusicPlayer::ToggleActive(){

    this->SoundPlayer::ToggleActive();
    if(this->active){
        Mix_VolumeMusic(this->CalculateActualVolume());
        Mix_ResumeMusic();
    }
    else Mix_PauseMusic();

}

bool MusicPlayer::ResetVolume(){

    if(!this->SoundPlayer::ResetVolume()) return false;
    if(this->active) Mix_VolumeMusic(this->CalculateActualVolume());
    return true;

}

bool MusicPlayer::IncreaseVolume(){

    if(!this->SoundPlayer::IncreaseVolume()) return false;
    if(this->active) Mix_VolumeMusic(this->CalculateActualVolume());
    return true;

}

bool MusicPlayer::DecreaseVolume(){

    if(!this->SoundPlayer::DecreaseVolume()) return false;
    if(this->active) Mix_VolumeMusic(this->CalculateActualVolume());
    return true;

}

void MusicPlayer::Play(){

    Mix_Music *const music = (game->Starting() ? NULL :
                              game->finished ? MusicPlayer::finished :
                              game->sun == NULL ? MusicPlayer::night_time :
                              MusicPlayer::day_time);
    if(this->music == music) return;
    this->music = music;
    if(this->music == NULL) Mix_HaltMusic();
    else Mix_PlayMusic(this->music, -1);

}





Mix_Chunk *EffectPlayer::creation;
Mix_Chunk *EffectPlayer::stock;
Mix_Chunk *EffectPlayer::lamp;
Mix_Chunk *EffectPlayer::collapse;
Mix_Chunk *EffectPlayer::fall;
Mix_Chunk *EffectPlayer::disaster;
Mix_Chunk *EffectPlayer::simple;

void EffectPlayer::Initialize(bool new_year){

    Mix_AllocateChannels(32);
    char fn[FILENAME_MAX + 1];
    EffectPlayer::creation = Mix_LoadWAV(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "creation.ogg")
    );
    EffectPlayer::stock = Mix_LoadWAV(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "stock.ogg")
    );
    EffectPlayer::lamp = Mix_LoadWAV(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "lamp.ogg")
    );
    EffectPlayer::collapse = Mix_LoadWAV(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "collapse.ogg")
    );
    EffectPlayer::fall = Mix_LoadWAV(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "fall.ogg")
    );
    EffectPlayer::disaster = Mix_LoadWAV(
        strcat(strcpy(fn, SoundPlayer::sound_folder), "disaster.ogg")
    );
    EffectPlayer::simple = Mix_LoadWAV(
        strcat(strcpy(fn, SoundPlayer::sound_folder),
               new_year ? "0.ogg" : "simple.ogg")
    );

}

void EffectPlayer::Terminate(){

    Mix_FreeChunk(EffectPlayer::creation);
    Mix_FreeChunk(EffectPlayer::stock);
    Mix_FreeChunk(EffectPlayer::lamp);
    Mix_FreeChunk(EffectPlayer::collapse);
    Mix_FreeChunk(EffectPlayer::fall);
    Mix_FreeChunk(EffectPlayer::disaster);
    Mix_FreeChunk(EffectPlayer::simple);

}

EffectPlayer::EffectPlayer(void (*TimerFunction)(int)) :
SoundPlayer(TimerFunction){}

EffectPlayer::EffectPlayer(FILE *f, void (*TimerFunction)(int)) :
SoundPlayer(f, TimerFunction){}

EffectPlayer::~EffectPlayer(){

    Mix_HaltChannel(-1);

}

int EffectPlayer::Play(Game::Event event){

    if(!this->active) return -1;
    int channel = Mix_GroupAvailable(-1);
    if(channel == -1){
        channel = Mix_GroupOldest(-1);
        game->player->ReleaseChannel(channel);
        Mix_HaltChannel(channel);
    }
    Mix_Volume(channel, this->CalculateActualVolume());
    if(event == Game::POSITION_RESET || event == Game::COLLISION)
        Mix_PlayChannel(channel, EffectPlayer::disaster, 0);
    else if(event == Game::CUBE_CREATED)
        Mix_PlayChannel(channel, EffectPlayer::creation, 0);
    else if(event == Game::CUBE_OBTAINED)
        Mix_PlayChannel(channel, EffectPlayer::stock, 0);
    else if(event == Game::COLUMN_DESTROYED)
        Mix_PlayChannel(channel, EffectPlayer::lamp, 0);
    else if(event == Game::PLAYER_FALLING)
        Mix_PlayChannel(channel, EffectPlayer::fall, -1);
    else if(event == Game::CUBES_COLLAPSING)
        Mix_PlayChannel(channel, EffectPlayer::collapse, 0);
    else if(event == Game::NEUTRAL)
        Mix_PlayChannel(channel, EffectPlayer::simple, 0);
    return channel;

}

void EffectPlayer::Stop(int channel, int time){

    if(channel == -1) return;
    if(time == 0) Mix_HaltChannel(channel);
    else Mix_FadeOutChannel(channel, time);

}





Pointer::Pointer(void (*TimerFunction)(int)) :
AutoHidable(TimerFunction){

    this->active = true;
    this->tolerance = glutGet(GLUT_SCREEN_HEIGHT);
    int const width = glutGet(GLUT_SCREEN_WIDTH);
    if(this->tolerance > width) this->tolerance = width;
    this->tolerance /= 300;
    if(this->tolerance < 1) this->tolerance = 1;
    this->x = this->y = -1;
    this->x_ = this->y_ = 0;

}

Pointer::Pointer(FILE *f, void (*TimerFunction)(int)) :
AutoHidable(TimerFunction){

    this->active = fgetc(f) - '0';
    fgetc(f);
    fscanf(f, "%d", &this->tolerance);
    fgetc(f);
    this->x = this->y = -1;
    this->x_ = this->y_ = 0;

}

void Pointer::Store(FILE *f){

    fprintf(f, "%d %d\n", this->active, this->tolerance);

}

void Pointer::Toggle(){

    this->ToggleActive();

}

void Pointer::Reset(){

    this->ResetTolerance();

}

void Pointer::Increase(){

    this->IncreaseTolerance();

}

void Pointer::Decrease(){

    this->DecreaseTolerance();

}

bool Pointer::MakeVisible(){

    if(!this->AutoHidable::MakeVisible()) return false;
    glutSetCursor(GLUT_CURSOR_INHERIT);
    return true;

}

bool Pointer::MakeInvisible(){

    if(!this->AutoHidable::MakeInvisible()) return false;
    glutSetCursor(GLUT_CURSOR_NONE);
    return true;

}

void Pointer::IncreaseTimers(){

    if(this->active || !this->Positioned()) return;
    this->AutoHidable::IncreaseTimers();

}

void Pointer::Show(){

    glutPassiveMotionFunc(this->active ?
                          ActivePassiveMotionFunction :
                          InactivePassiveMotionFunction);
    if(this->Positioned())
        glutSetCursor(this->active ? GLUT_CURSOR_CROSSHAIR :
                      this->visible ? GLUT_CURSOR_INHERIT :
                      GLUT_CURSOR_NONE);

}

void Pointer::Hide(){

    glutPassiveMotionFunc(NULL);
    if(this->Positioned()) glutSetCursor(GLUT_CURSOR_NONE);

}

bool Pointer::Positioned(){

    return this->x != -1 && this->y != -1;

}

bool Pointer::Active(){

    return this->active;

}

void Pointer::ToggleActive(){

    this->active = !this->active;
    this->AutoHidable::MakeInvisible();
    this->Show();
    if(!this->Positioned()) return;
    if(!this->active){
        this->x += this->x_;
        this->y += this->y_;
        this->x_ = this->y_ = 0;
    }
    cfg->display->PostRedisplay();

}

int Pointer::Tolerance(){

    return this->tolerance;

}

bool Pointer::ResetTolerance(){

    int def = glutGet(GLUT_SCREEN_HEIGHT);
    int const width = glutGet(GLUT_SCREEN_WIDTH);
    if(def > width) def = width;
    def /= 300;
    if(def < 1) def = 1;
    if(this->tolerance == def) return false;
    this->tolerance = def;
    if(this->active && this->Positioned()){
        if(abs(this->x_) >= this->tolerance){
            this->x += this->x_;
            this->x -= this->x_ %= this->tolerance;
        }
        if(abs(this->y_) >= this->tolerance){
            this->y += this->y_;
            this->y -= this->y_ %= this->tolerance;
        }
        cfg->display->PostRedisplay();
    }
    return true;

}

bool Pointer::IncreaseTolerance(){

    if(this->tolerance >= glutGet(GLUT_SCREEN_WIDTH) &&
       this->tolerance >= glutGet(GLUT_SCREEN_HEIGHT))
        return false;
    this->tolerance++;
    if(this->active && this->Positioned())
        cfg->display->PostRedisplay();
    return true;

}

bool Pointer::DecreaseTolerance(){

    if(this->tolerance == 1) return false;
    this->tolerance--;
    if(this->active && this->Positioned()){
        if(abs(this->x_) == this->tolerance){
            this->x += this->x_;
            this->x_ = 0;
        }
        if(abs(this->y_) == this->tolerance){
            this->y += this->y_;
            this->y_ = 0;
        }
        cfg->display->PostRedisplay();
    }
    return true;

}





void Zoom::Reset(){

    if(game->camera->ResetZoom()) cfg->display->PostRedisplay();

}

void Zoom::Increase(){

    if(game->camera->IncreaseZoom()) cfg->display->PostRedisplay();

}

void Zoom::Decrease(){

    if(game->camera->DecreaseZoom()) cfg->display->PostRedisplay();

}





void HorizontalTilt::Reset(){

    if(game->camera->ResetHorizontalTilt()) cfg->display->PostRedisplay();

}

void HorizontalTilt::Increase(){

    int i;
    for(i = 0; i < 5 && game->camera->IncreaseHorizontalTilt(); i++);
    if(i != 0) cfg->display->PostRedisplay();

}

void HorizontalTilt::Decrease(){

    int i;
    for(i = 0; i < 5 && game->camera->DecreaseHorizontalTilt(); i++);
    if(i != 0) cfg->display->PostRedisplay();

}





void VerticalTilt::Reset(){

    if(game->camera->ResetVerticalTilt()) cfg->display->PostRedisplay();

}

void VerticalTilt::Increase(){

    int i;
    for(i = 0; i < 5 && game->camera->IncreaseVerticalTilt(); i++);
    if(i != 0) cfg->display->PostRedisplay();

}

void VerticalTilt::Decrease(){

    int i;
    for(i = 0; i < 5 && game->camera->DecreaseVerticalTilt(); i++);
    if(i != 0) cfg->display->PostRedisplay();

}





GameIO::GameIO(){

    this->filename = new char[FILENAME_MAX - strlen(storage_folder) + 1];
    this->filename[0] = '\0';
    this->length = this->cursor = this->visible_start = 0;

}

GameIO::~GameIO(){

    delete[] this->filename;

}

void GameIO::DoIO(){

    cfg->ExitGameIO();

}

bool GameIO::MoveLeft(){

    if(this->cursor == 0) return false;
    this->cursor--;
    if(this->visible_start > this->cursor)
        this->visible_start = this->cursor;
    cfg->display->PostRedisplay();
    return true;

}

bool GameIO::MoveRight(){

    if(this->cursor == this->length) return false;
    this->cursor++;
    if(this->visible_start < this->cursor - 19)
        this->visible_start = this->cursor - 19;
    cfg->display->PostRedisplay();
    return true;

}

bool GameIO::GoToStart(){

    if(this->cursor == 0) return false;
    this->cursor = 0;
    if(this->visible_start > this->cursor)
        this->visible_start = this->cursor;
    cfg->display->PostRedisplay();
    return true;

}

bool GameIO::GoToEnd(){

    if(this->cursor == this->length) return false;
    this->cursor = this->length;
    if(this->visible_start < this->cursor - 19)
        this->visible_start = this->cursor - 19;
    cfg->display->PostRedisplay();
    return true;

}

bool GameIO::Insert(char c){

    if(this->length == FILENAME_MAX - strlen(storage_folder)) return false;
    int i;
    char c_next;
    for(i = this->cursor; i <= this->length; i++){
        c_next = this->filename[i];
        this->filename[i] = c;
        c = c_next;
    }
    this->filename[i] = c;
    this->length++;
    cfg->display->PostRedisplay();
    return true;

}

bool GameIO::Delete(){

    if(this->cursor == this->length) return false;
    for(int i = this->cursor; i < this->length; i++)
        this->filename[i] = this->filename[i + 1];
    this->length--;
    cfg->display->PostRedisplay();
    return true;

}

void GameIO::Visualize(){

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(5.0, 5.0, 0.0);
    glRecti(0, 0, 1, 25);
    glRecti(189, 0, 190, 25);
    glRecti(1, 0, 189, 1);
    glRecti(1, 24, 189, 25);
    glTranslatef(5.0, 0.0, 0.0);
    glRecti((this->cursor - this->visible_start) * 9, 1,
            (this->cursor - this->visible_start + 1) * 9, 4);
    glRecti((this->cursor - this->visible_start) * 9, 21,
            (this->cursor - this->visible_start + 1) * 9, 24);
    glTranslatef(0.0, 5.0, 0.0);
    int i;
    for(i = 0; i < 20 && i < this->length - this->visible_start; i++){
        glRasterPos2i(9 * i, 4);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,
                            this->filename[this->visible_start + i]);
    }
    glRecti(9 * i, 0, 180, 15);
    glPopMatrix();

    glEnable(GL_BLEND);
    glColor4f(0.25, 0.25, 0.25, 0.5);
    glRecti(0, 0, 200, 35);
    glDisable(GL_BLEND);

}





void GameLoader::DoIO(){

    if(LoadGame(this->filename)) this->GameIO::DoIO();

}





void GameStorer::DoIO(){

    if(StoreGame(this->filename)) this->GameIO::DoIO();

}





void Configuration::Initialize(bool new_year){

    SDL_Init(0);
    Display::Initialize();
    Window::Initialize();
    SoundPlayer::Initialize();
    MusicPlayer::Initialize(new_year);
    EffectPlayer::Initialize(new_year);

}

void Configuration::Terminate(){

    MusicPlayer::Terminate();
    EffectPlayer::Terminate();
    SoundPlayer::Terminate();
    SDL_Quit();

}

Configuration::Configuration(){

    this->game_info = this->player_info = true;
    this->display = this->window = new Window();
    this->full_screen = new FullScreen();
    this->adjustment = this->music_player = new MusicPlayer(MusicPlayerTimerFunction);
    this->effect_player = new EffectPlayer(EffectPlayerTimerFunction);
    this->pointer = new Pointer(PointerTimerFunction);
    this->zoom = new Zoom();
    this->horizontal_tilt = new HorizontalTilt();
    this->vertical_tilt = new VerticalTilt();
    this->game_i_o = NULL;

}

Configuration::Configuration(FILE *f){

    int option;
    this->game_info = fgetc(f) - '0';
    fgetc(f);
    this->player_info = fgetc(f) - '0';
    fgetc(f);
    this->window = new Window(f);
    this->full_screen = new FullScreen(f);
    fscanf(f, "%d", &option);
    fgetc(f);
    if(option == 0) this->display = this->window;
    else if(option == 1) this->display = this->full_screen;
    this->music_player = new MusicPlayer(f, MusicPlayerTimerFunction);
    this->effect_player = new EffectPlayer(f, EffectPlayerTimerFunction);
    this->pointer = new Pointer(f, PointerTimerFunction);
    this->zoom = new Zoom();
    this->horizontal_tilt = new HorizontalTilt();
    this->vertical_tilt = new VerticalTilt();
    fscanf(f, "%d", &option);
    fgetc(f);
    if(option == 0) this->adjustment = this->music_player;
    else if(option == 1) this->adjustment = this->effect_player;
    else if(option == 2) this->adjustment = this->pointer;
    else if(option == 3) this->adjustment = this->zoom;
    else if(option == 4) this->adjustment = this->horizontal_tilt;
    else if(option == 5) this->adjustment = this->vertical_tilt;
    this->game_i_o = NULL;

}

Configuration::~Configuration(){

    delete this->window;
    delete this->full_screen;
    delete this->music_player;
    delete this->effect_player;
    delete this->pointer;
    delete this->zoom;
    delete this->horizontal_tilt;
    delete this->vertical_tilt;
    delete this->game_i_o;

}

void Configuration::Store(FILE *f){

    fprintf(f, "%d\n", this->game_info);
    fprintf(f, "%d\n", this->player_info);
    this->window->Store(f);
    this->full_screen->Store(f);
    if(this->display == this->window) fprintf(f, "%d\n", 0);
    else if(this->display == this->full_screen) fprintf(f, "%d\n", 1);
    this->music_player->Store(f);
    this->effect_player->Store(f);
    this->pointer->Store(f);
    if(this->adjustment == this->music_player) fprintf(f, "%d\n", 0);
    else if(this->adjustment == this->effect_player) fprintf(f, "%d\n", 1);
    else if(this->adjustment == this->pointer) fprintf(f, "%d\n", 2);
    else if(this->adjustment == this->zoom) fprintf(f, "%d\n", 3);
    else if(this->adjustment == this->horizontal_tilt) fprintf(f, "%d\n", 4);
    else if(this->adjustment == this->vertical_tilt) fprintf(f, "%d\n", 5);

}

void Configuration::ToggleGameInfo(){

    this->game_info = !this->game_info;
    this->display->PostRedisplay();

}

void Configuration::TogglePlayerInfo(){

    this->player_info = !this->player_info;
    if(game->player != NULL) this->display->PostRedisplay();

}

void Configuration::ChangeDisplay(){

    int x = this->pointer->x, y = this->pointer->y;
    int x_ = this->pointer->x_, y_ = this->pointer->y_;
    if(!this->display->Hide()) return;
    this->display->GlobalizePointerPosition(&x, &y, &x_, &y_);
    this->display = (this->display == this->window ?
                     (Display *)this->full_screen :
                     (Display *)this->window);
    this->display->LocalizePointerPosition(&x, &y, &x_, &y_);
    this->display->Show(x, y, x_, y_);

}

void Configuration::SetAdjustment(Adjustment *adjustment){

    AutoHidable *const auto_hidable = dynamic_cast<AutoHidable *>(adjustment);
    if(auto_hidable != NULL) auto_hidable->IncreaseTimers();
    if(this->adjustment == adjustment) return;
    this->adjustment = adjustment;

}

void Configuration::CreateGameLoader(){

    this->game_i_o = new GameLoader();
    glutKeyboardFunc(IOKeyboardFunction);
    glutSpecialFunc(IOSpecialFunction);
    this->display->PostRedisplay();

}

void Configuration::CreateGameStorer(){

    this->game_i_o = new GameStorer();
    glutKeyboardFunc(IOKeyboardFunction);
    glutSpecialFunc(IOSpecialFunction);
    this->display->PostRedisplay();

}

void Configuration::ExitGameIO(){

    delete this->game_i_o;
    this->game_i_o = NULL;
    glutKeyboardFunc(MainKeyboardFunction);
    glutSpecialFunc(MainSpecialFunction);
    this->display->PostRedisplay();

}

void Configuration::Visualize(){

    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, this->display->width, 0.0, this->display->height, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -1.0);

    glDisable(GL_LIGHTING);
    if(game->finished){
        glTranslatef(0.0, 0.0, 0.01);
        this->display->VisualizeDimLayer();
        glTranslatef(0.0, 0.0, 0.01);
        this->display->VisualizeFinished();
    }
    if(this->music_player->Visible()){
        glTranslatef(0.0, 0.0, 0.01);
        glPushMatrix();
        glTranslatef(5.0, this->display->height - 122.0, 0.0);
        this->music_player->Visualize();
        glPopMatrix();
    }
    if(this->effect_player->Visible()){
        glTranslatef(0.0, 0.0, 0.01);
        glPushMatrix();
        glTranslatef(this->display->width - 25.0, this->display->height - 122.0, 0.0);
        this->effect_player->Visualize();
        glPopMatrix();
    }
    bool const player_info = this->player_info && game->player != NULL;
    if(this->game_info || player_info){
        glTranslatef(0.0, 0.0, 0.01);
        glPushMatrix();
        glTranslatef(5.0, 5.0, 0.0);
        if(this->game_info) game->VisualizeInfo();
        if(this->game_info && player_info) glTranslatef(0.0, 55.0, 0.0);
        if(player_info) game->player->VisualizeInfo();
        glPopMatrix();
    }
    if(this->game_i_o != NULL){
        glTranslatef(0.0, 0.0, 0.01);
        glPushMatrix();
        glTranslatef(this->display->width - 205.0, 5.0, 0.0);
        this->game_i_o->Visualize();
        glPopMatrix();
    }
    if(this->pointer->Active() && this->pointer->Positioned()){
        glTranslatef(0.0, 0.0, 0.01);
        game->camera->VisualizeTiltFrame();
    }
    glEnable(GL_LIGHTING);

}





char const *const storage_folder = "..\\storage\\";
Game *game;
Configuration *cfg;

void Initialize(int argc, char **argv){

    atexit(Terminate);
    glutInit(&argc, argv);

    time_t const t = time(NULL);
    Game::Initialize(t, argc, argv);
    Configuration::Initialize(localtime(&t)->tm_yday == 0);

    char fn[FILENAME_MAX + 1];
    FILE *const f = fopen(strcat(strcpy(fn, storage_folder), "configuration"), "r");
    if(f == NULL) cfg = new Configuration();
    else{
        cfg = new Configuration(f);
        fclose(f);
    }
    for(int i = 1; i < argc; i++){
        if(strlen(argv[i]) != 3 || argv[i][1] != '=') continue;
        Display *const display = (argv[i][0] == 'w' ?
                                  (Display *)cfg->window :
                                  argv[i][0] == 'f' ?
                                  (Display *)cfg->full_screen :
                                  NULL);
        if(display == NULL) continue;
        if(argv[i][2] == 's') display->rendering = Display::SINGLE;
        else if(argv[i][2] == 'd') display->rendering = Display::DOUBLE;
        else if(argv[i][2] == 'a') display->rendering = Display::AVSYNC;
        else if(argv[i][2] == 'v') display->rendering = Display::VSYNC;
    }
    cfg->display->Show();

    for(int i = 1; i < argc; i++){
        if(strlen(argv[i]) < 3 || argv[i][0] != 's' || argv[i][1] != '=') continue;
        LoadGame(argv[i] + 2);
    }
    if(game == NULL) StartGame();

}

void Terminate(){

    delete game;
    char fn[FILENAME_MAX + 1];
    FILE *const f = fopen(strcat(strcpy(fn, storage_folder), "configuration"), "w");
    if(f != NULL){
        cfg->Store(f);
        fclose(f);
    }
    delete cfg;
    Configuration::Terminate();

}

void StartGame(){

    delete game;
    game = new Game();
    cfg->music_player->Play();
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();
    SetTimer(Start0TimerFunction, 666);
    Game::start_timers++;

}

bool LoadGame(char const *filename){

    char fn[FILENAME_MAX + 1];
    FILE *const f = fopen(strcat(strcpy(fn,
                                        strchr(filename, '\\') == filename ||
                                        strchr(filename, ':') != NULL ?
                                        "" :
                                        storage_folder),
                                 filename),
                          "r");
    if(f == NULL) return false;
    float a, d, r;
    int m;
    fscanf(f, "%f%f%f%d", &a, &d, &r, &m);
    fgetc(f);
    if(a != Game::a || d != Game::d || r != Game::r || m != Game::m){
        fclose(f);
        return false;
    }
    delete game;
    game = new Game(f);
    cfg->music_player->Play();
    cfg->effect_player->Play(Game::NEUTRAL);
    cfg->display->PostRedisplay();
    fclose(f);
    return true;

}

bool StoreGame(char const *filename){

    if(game->Evolving()) return false;
    char fn[FILENAME_MAX + 1];
    FILE *const f = fopen(strcat(strcpy(fn,
                                        strchr(filename, '\\') == filename ||
                                        strchr(filename, ':') != NULL ?
                                        "" :
                                        storage_folder),
                                 filename),
                          "w");
    if(f == NULL) return false;
    cfg->effect_player->Play(Game::NEUTRAL);
    SetTimer(StoreTimerFunction, 100);
    fprintf(f, "%f %f %f %d\n", Game::a, Game::d, Game::r, Game::m);
    game->Store(f);
    fclose(f);
    return true;

}

int ElapsedTime(){

    return glutGet(GLUT_ELAPSED_TIME);

}

void ActivateAnimation(){

    glutIdleFunc(IdleFunction);

}

void SetTimer(void (*TimerFunction)(int), int time){

    glutTimerFunc(time, TimerFunction, 0);

}

void WindowInitDisplayFunction(){

    glutPositionFunc(MainPositionFunction);
    glutReshapeFunc(ReshapeFunction);
    InitDisplayFunction();

}

void InitDisplayFunction(){

    cfg->display->showing = false;
    glutDisplayFunc(MainDisplayFunction);
    MainDisplayFunction();

}

void MainDisplayFunction(){

    game->Visualize();
    cfg->Visualize();
    (cfg->display->rendering == Display::SINGLE ?
     glFlush :
     glutSwapBuffers)();

}

void InitPositionFunction(int x, int y){

    if(y < 0) SetTimer(InitRepairTimerFunction);
    glutPositionFunc(NULL);

}

void MainPositionFunction(int x, int y){

    if(y < 0){
        if(cfg->window->damaged) return;
        cfg->window->damaged = true;
        glutReshapeFunc(NULL);
        cfg->pointer->Hide();
        if(cfg->window->repair_scheduled) return;
        SetTimer(MainRepairTimerFunction);
        cfg->window->repair_scheduled = true;
        return;
    }
    if(cfg->window->damaged){
        cfg->window->damaged = false;
        glutReshapeFunc(ReshapeFunction);
        cfg->pointer->Show();
    }
    cfg->window->x = x;
    cfg->window->y = y;

}

void ReshapeFunction(int width, int height){

    cfg->window->width = width;
    cfg->window->height = height;
    glViewport(0, 0, cfg->window->width, cfg->window->height);

}

void IOKeyboardFunction(unsigned char key, int x, int y){

    if(key == 13)
        if((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == 0)
            cfg->game_i_o->DoIO();
        else cfg->ExitGameIO();
    else if(key == 127) cfg->game_i_o->Delete();
    else if(key == '\b'){
        if(cfg->game_i_o->MoveLeft()) cfg->game_i_o->Delete();
    }
    else if(isprint(key)){
        if(cfg->game_i_o->Insert(key)) cfg->game_i_o->MoveRight();
    }
    else MainKeyboardFunction(key, x, y);

}

void MainKeyboardFunction(unsigned char key, int x, int y){

    if(key == 13)
        (game->player->*((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == 0 ?
                         Player::CreateCube :
                         Player::CreateLamp))();
    else if(key == 27) exit(EXIT_SUCCESS);
    else if(key == ' ') game->player->ObtainCube();
    else{
        key = tolower(key);
        if(key == 'x') game->player->DestroyColumn();
        else if(key == 'd') game->player->DestroyCube();
        else if(key == 'c') game->StartCollapse();
        else if(key == 'f') game->player->PushCube();
        else if(key == 'n') StartGame();
        else if(key == 'm')
            if((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == 0)
                LoadGame();
            else cfg->CreateGameLoader();
        else if(key == 'b')
            if((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == 0)
                StoreGame();
            else cfg->CreateGameStorer();
        else if(key == 'q') exit(EXIT_SUCCESS);
    }

}

void IOSpecialFunction(int key, int x, int y){

    if(key == GLUT_KEY_DOWN) cfg->game_i_o->GoToStart();
    else if(key == GLUT_KEY_UP) cfg->game_i_o->GoToEnd();
    else if(key == GLUT_KEY_LEFT) cfg->game_i_o->MoveLeft();
    else if(key == GLUT_KEY_RIGHT) cfg->game_i_o->MoveRight();
    else MainSpecialFunction(key, x, y);

}

void MainSpecialFunction(int key, int x, int y){

    if(key == GLUT_KEY_UP) game->player->StepFront();
    else if(key == GLUT_KEY_DOWN) game->player->StepBack();
    else if(key == GLUT_KEY_RIGHT) game->player->TurnRight();
    else if(key == GLUT_KEY_LEFT) game->player->TurnLeft();
    else if(key == GLUT_KEY_F7) game->UseChangeStillCamera();
    else if(key == GLUT_KEY_F8) game->UsePlayerCamera();
    else if(key == GLUT_KEY_F9)
        (game->*((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == 0 ?
                 Game::ChangeGridLines :
                 Game::ChangeGridPlanes))();
    else if(key == GLUT_KEY_F1) cfg->SetAdjustment(cfg->music_player);
    else if(key == GLUT_KEY_F2) cfg->SetAdjustment(cfg->effect_player);
    else if(key == GLUT_KEY_F3) cfg->SetAdjustment(cfg->pointer);
    else if(key == GLUT_KEY_F4) cfg->SetAdjustment(cfg->zoom);
    else if(key == GLUT_KEY_F5) cfg->SetAdjustment(cfg->horizontal_tilt);
    else if(key == GLUT_KEY_F6) cfg->SetAdjustment(cfg->vertical_tilt);
    else if(key == GLUT_KEY_F11) cfg->ChangeDisplay();
    else if(key == GLUT_KEY_F12)
        (cfg->*((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == 0 ?
                Configuration::TogglePlayerInfo :
                Configuration::ToggleGameInfo))();
    else if(key == GLUT_KEY_HOME) cfg->adjustment->Reset();
    else if(key == GLUT_KEY_PAGE_UP) cfg->adjustment->Increase();
    else if(key == GLUT_KEY_PAGE_DOWN) cfg->adjustment->Decrease();
    else if(key == GLUT_KEY_END) cfg->adjustment->Toggle();

}

void MouseFunction(int button, int state, int x, int y){

    if(state == GLUT_UP) return;
    if(button == GLUT_LEFT_BUTTON) game->UseChangeStillCamera();
    else if(button == GLUT_RIGHT_BUTTON) game->UsePlayerCamera();
    else if(button == GLUT_MIDDLE_BUTTON) cfg->pointer->ToggleActive();

}

void MouseWheelFunction(int wheel, int direction, int x, int y){

    if((game->camera->*(direction == 1 ?
                        Camera::IncreaseZoom :
                        Camera::DecreaseZoom))())
        cfg->display->PostRedisplay();

}

void EntryFunction(int state){

    if(state == GLUT_ENTERED) return;
    cfg->pointer->AutoHidable::MakeInvisible();
    cfg->pointer->x = cfg->pointer->y = -1;
    if(!cfg->pointer->Active()) return;
    cfg->pointer->x_ = cfg->pointer->y_ = 0;
    cfg->display->PostRedisplay();

}

void InactivePassiveMotionFunction(int x, int y){

    if(cfg->pointer->x == x && cfg->pointer->y == y) return;
    cfg->pointer->x = x;
    cfg->pointer->y = y;
    cfg->pointer->IncreaseTimers();

}

void ActivePassiveMotionFunction(int x, int y){

    if(cfg->pointer->x + cfg->pointer->x_ == x &&
       cfg->pointer->y + cfg->pointer->y_ == y)
        return;
    if(cfg->pointer->Positioned()){

        int i;
        div_t r;

        r = div(x - cfg->pointer->x, cfg->pointer->Tolerance());
        cfg->pointer->x_ = r.rem;
        cfg->pointer->x = x - cfg->pointer->x_;
        for(i = 0; i < r.quot && game->camera->IncreaseHorizontalTilt(); i++);
        for(i = 0; i > r.quot && game->camera->DecreaseHorizontalTilt(); i--);

        r = div(y - cfg->pointer->y, cfg->pointer->Tolerance());
        cfg->pointer->y_ = r.rem;
        cfg->pointer->y = y - cfg->pointer->y_;
        for(i = 0; i < r.quot && game->camera->DecreaseVerticalTilt(); i++);
        for(i = 0; i > r.quot && game->camera->IncreaseVerticalTilt(); i--);

    }
    else{
        cfg->pointer->x = x;
        cfg->pointer->y = y;
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    }
    cfg->display->PostRedisplay();

}

void IdleFunction(){

    if(game->player == NULL || !game->player->ContinueMove())
        glutIdleFunc(NULL);

}

void InitRepairTimerFunction(int _){

    cfg->window->showing = false;
    cfg->window->Repair();

}

void MainRepairTimerFunction(int _){

    cfg->window->repair_scheduled = false;
    if(!cfg->window->damaged) return;
    cfg->window->damaged = false;
    cfg->window->Repair();

}

void MusicPlayerTimerFunction(int _){

    cfg->music_player->DecreaseTimers();

}

void EffectPlayerTimerFunction(int _){

    cfg->effect_player->DecreaseTimers();

}

void PointerTimerFunction(int _){

    cfg->pointer->DecreaseTimers();

}

void StoreTimerFunction(int _){

    cfg->effect_player->Play(Game::NEUTRAL);

}

void Start0TimerFunction(int _){

    Game::start_timers--;
    if(Game::start_timers > 0 || !game->ContinueStart0()) return;
    SetTimer(Start1TimerFunction, 666);
    Game::start_timers++;

}

void Start1TimerFunction(int _){

    Game::start_timers--;
    if(Game::start_timers > 0 || !game->ContinueStart1()) return;
    SetTimer(Start2TimerFunction, 666);
    Game::start_timers++;

}

void Start2TimerFunction(int _){

    Game::start_timers--;
    if(Game::start_timers > 0 || !game->ContinueStart2()) return;

}

void CollapseTimerFunction(int _){

    Game::collapse_timers--;
    if(Game::collapse_timers > 0 || !game->ContinueCollapse()) return;
    SetTimer(CollapseTimerFunction, 250);
    Game::collapse_timers++;

}





int main(int argc, char **argv){

    Initialize(argc, argv);
    glutMainLoop();
    return EXIT_FAILURE;

}
