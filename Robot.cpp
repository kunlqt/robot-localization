//
// Created by jannik on 04.09.18.
//

#include "Robot.h"
#include "SDL.h"
#include <random>


Robot::Robot(int x_start, int y_start, float orientation, int rad, SDL_Color col)
{
    // state properties
    pose.x = x_start;
    pose.y = y_start;
    pose.phi = orientation;

    velocity.x = 0.;
    velocity.y = 0.;
    velocity.phi = 0.;

    // static properties
    radius = rad;
    color = col;
}

Robot::~Robot()
{

}


void Robot::render(SDL_Renderer * ren)
{
    int32_t x = radius - 1;
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t err = tx - (radius << 1); // shifting bits left by 1 effectively
    // doubles the value. == tx - diameter

    int32_t int_pose_x = int(pose.x);
    int32_t int_pose_y = int(pose.y);

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);


    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(ren, int_pose_x + x, int_pose_y - y);
        SDL_RenderDrawPoint(ren, int_pose_x + x, int_pose_y + y);
        SDL_RenderDrawPoint(ren, int_pose_x - x, int_pose_y - y);
        SDL_RenderDrawPoint(ren, int_pose_x - x, int_pose_y + y);
        SDL_RenderDrawPoint(ren, int_pose_x + y, int_pose_y - x);
        SDL_RenderDrawPoint(ren, int_pose_x + y, int_pose_y + x);
        SDL_RenderDrawPoint(ren, int_pose_x - y, int_pose_y - x);
        SDL_RenderDrawPoint(ren, int_pose_x - y, int_pose_y + x);

        if (err <= 0)
        {
            y++;
            err += ty;
            ty += 2;
        }
        if (err > 0)
        {
            x--;
            tx += 2;
            err += tx - (radius << 1);
        }
    }

    int x0 = int_pose_x;
    int y0 = int_pose_y;
    int x1 = x0 + int(15*(cos(pose.phi) - sin(pose.phi)));
    int y1 = y0 + int(15*(sin(pose.phi) + cos(pose.phi)));

    SDL_RenderDrawLine(ren, x0, y0, x1, y1);


}


void Robot::move(const Uint8 * state)
{
    if (state[SDL_SCANCODE_RIGHT])
    {
        printf("rotating right\n");
        rotateRight();
    }


    if (state[SDL_SCANCODE_LEFT])
    {
        printf("rotating left\n");
        rotateLeft();
    }


    if (state[SDL_SCANCODE_UP])
    {
        printf("moving forward\n");
        moveForward();
    }


    if (state[SDL_SCANCODE_DOWN])
    {
        printf("moving backward\n");
        moveBackward();
    }

}



void Robot::moveForward() {

    velocity.x = DT * (cos(pose.phi) - sin(pose.phi));
    velocity.y = DT * (sin(pose.phi) + cos(pose.phi));

    pose.x += velocity.x;
    pose.y += velocity.y;
}

void Robot::moveBackward(){

    velocity.x = - DT * (cos(pose.phi) - sin(pose.phi));
    velocity.y = - DT * (sin(pose.phi) + cos(pose.phi));


    pose.x += velocity.x;
    pose.y += velocity.y;
}


void Robot::rotateLeft() {


    velocity.phi = - DT * 2*M_PI/360;;
    pose.phi += velocity.phi;


}

void Robot::rotateRight() {

    velocity.phi = DT * 2*M_PI/360;;
    pose.phi += velocity.phi;
}


void Robot::setPose(float x, float y, float phi) {

    pose.x = x;
    pose.y = y;
    pose.phi = phi;
}



std::vector<Landmark> Robot::measureLandmarks(std::vector<Landmark> landmarks)
{
    std::vector<Landmark> measured_landmarks;

    uint32_t time_ui = uint32_t( time(NULL) );
    srand( time_ui );

    for (auto lm = landmarks.begin(); lm != landmarks.end(); ++lm)
    {
        // Define random generator with Gaussian distribution

        float rx = 5.0*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float ry = 5.0*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        float est_pos_x = rx + lm->pos.x;
        float est_pos_y = ry + lm->pos.y;

        measured_landmarks.push_back(Landmark(est_pos_x,est_pos_y,lm->id));
    }

    return measured_landmarks;

}



Eigen::VectorXf Robot::get_state()
{
    Eigen::VectorXf state(2);

    // with PHI
//    state(0) = this->pose.x;
//    state(1) = this->pose.y;
//    state(2) = this->pose.phi;
//    state(3) = this->velocity.x;
//    state(4) = this->velocity.y;
//    state(5) = this->velocity.phi;

    // without PHI
    state(0) = this->pose.x;
    state(1) = this->pose.y;


    return state;
}
