#ifndef __WANDERER_AGENT__H
#define __WANDERER_AGENT__H 

#include <stdio.h>
#include <string>
#include <math.h>
#include "enviro.h"

namespace {

    using namespace enviro;

    /*The MovingForward state enables the robot to move towards a particular objective.
    */
    class MovingForward : public State, public AgentInterface {
        public:
        void entry(const Event& e) {
        }
        void during() { 
             move_toward(goal_x, goal_y, 30, 20);
            //if the distance between the robot front sensor and object is less than 60, emit Event
            if ( sensor_value(0) < 60 ) {
                emit(Event(tick_name));
            } 
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        //If the scree is clicked set the target position to let the robot move
        void set_goal(double x, double y) { 
            goal_x = x;
            goal_y = y;
         }
        std::string tick_name;
        double goal_x, goal_y;
    };

    /*In the Stopping state, the robot's velocity is set to zero, 
    but it continues to rotate to prevent getting stuck in a corner.
    */
    class Stopping : public State, public AgentInterface {
        public:
        void entry(const Event& e) {
            //add label to robot and tell user to reclick the screen
            decorate("<circle x='-5' y='5' r='5' style='fill: red'></circle>");
            label("reclick screen", 20, 5);
        }
        void during() { 
            //change the speed to 0
            track_velocity(0,0,0,0);
            damp_movement();
            emit(Event(tick_name));
        }              
        void exit(const Event& e) {
            decorate(""); 
            clear_label();
        }
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    /*In the Restarting state, the robot utilizes teleportation 
    to relocate itself to its default starting position.
    */
    class Restarting : public State, public AgentInterface {
        public:
        void entry(const Event& e) {}
        void during() { 
            teleport(-600, -250, 1.57);
            emit(Event(tick_name));
        }
        void exit(const Event& e) {} 
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;  
    };

    /* The Wanderer Controller aims to link all states together and also monitors user interaction events.
    */
    class WandererController : public StateMachine, public AgentInterface {

        public:
        WandererController() : StateMachine() {

            set_initial(moving_forward);
            tick_name = "tick_" + std::to_string(rand()%1000); // use an agent specific generated 
                                                               // event name in case there are 
                                                               // multiple instances of this class
            add_transition(tick_name, moving_forward, stopping);
            add_transition(tick_name, stopping, moving_forward);
            add_transition("trigger_restart_button", stopping, restarting);
            add_transition("trigger_restart_button", moving_forward, restarting);
            add_transition(tick_name, restarting, moving_forward);

            moving_forward.set_tick_name(tick_name);
            stopping.set_tick_name(tick_name);
            restarting.set_tick_name(tick_name);

        }

        void init() {
            //watch the screen click
            watch("screen_click", [&](Event& e) {
                goal_x = e.value()["x"];
                goal_y = e.value()["y"];
                //set the target in the moving forward stage
                moving_forward.set_goal(goal_x, goal_y);
                //sending the location to follower robot
                emit(Event("sending_locations", {goal_x, goal_y}));
            });
            StateMachine::init();
        }

        void stop(){
            std::cout << "wc stop" << std::endl;
            StateMachine::stop();
        }

        MovingForward moving_forward;
        Stopping stopping;
        Restarting restarting;
        std::string tick_name;
        double goal_x, goal_y;
    };

    //This is the leading robot
    class Wanderer : public Agent {

        public:
        Wanderer(json spec, World& world) : Agent(spec, world) {
            add_process(wc);
        }

        WandererController wc;

    };

    DECLARE_INTERFACE(Wanderer);

}

#endif