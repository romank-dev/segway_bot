/* Copyright 2025 Roman Kudinov. All Rights Reserved.
 * Copyright 2025 Oded Yechiel. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include <cstdio>
#include <cstdlib>
#include <random>


/*  
Equations are based on the following link
https://underactuated.mit.edu/acrobot.html
*/

#define RAD2DEG 180.f / 3.14159f
#define DEG2RAD 3.14159f / 180.f


class State
{
public:
    float x, vx, theta, dtheta;
    void print_state()
    {
        printf("X      : %5.2f[m]\n", x);
        printf("Vx     : %5.2f[m/s]\n", vx);
        printf("Theta  : %5.2f[deg]\n", theta * RAD2DEG);
        printf("dTheta : %5.2f[deg/s]\n", dtheta * RAD2DEG);
        printf("---------------\n\n");
    }
};

class Command
{
public:
    float fx;
    Command(float f) : fx(f){}
    void print_state()
    {
        printf("Fx     : %5.2f[N]\n", fx);
        printf("---------------\n\n");
    }
};

class SegwayBot
{
public:
    SegwayBot()
    {
        _state.x = 0;
        _state.vx = 0;
        _state.theta = 3.f * DEG2RAD;
        _state.dtheta = 0;
    }

    State get_state()
    {
        return _state;
    }

    void step_10ms(Command cmd)
    {
        /*
            Segbot step is run at 10[ms].
            However, inner loop integration is done at 1[ms] for smooth discretization
        */
        float dt = 0.001;
        for (int integration_step = 0; integration_step < 10; integration_step++)
        {
            // Equations of Motion
            float dt2 = _state.dtheta * _state.dtheta;
            float st = sinf(_state.theta);
            float ct = cosf(_state.theta);
            float denom = 1. / (_mc + _mp * st * st);
            float d2x     = denom * (cmd.fx + _mp * st * (_l * dt2 + _g * ct));
            float d2theta = denom / _l * (-cmd.fx * ct - _mp * _l * dt2 * ct * st + _mp * _mc * _g * st);

            // Integrate state
            _state.x += _state.vx * dt;
            _state.theta += _state.dtheta * dt;
            _state.vx += d2x * dt;
            _state.dtheta += d2theta * dt;
        }
    }

    bool is_unrecoverable()
    {
        return fabs(_state.theta) > 30 * DEG2RAD;
    }

private:
    State _state;
    float _g        = 9.81; // m/s2
    float _l        = 0.5;  // m
    float _mc       = 2;    // kg
    float _mp       = 0.2;  // kg
};

int main()
{
    SegwayBot bot;
    State seg_state;
    Command cmd(0);
    float stopwatch = 0;
    while(!bot.is_unrecoverable())
    {
        bot.step_10ms(cmd);
        stopwatch += 0.01;
        seg_state = bot.get_state();
        printf("Time: %.2f\n", stopwatch);
        seg_state.print_state();
    }
}
