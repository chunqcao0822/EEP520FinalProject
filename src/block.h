#ifndef __DIFF_DRIVE_AGENT__H
#define __DIFF_DRIVE_AGENT__H 

#include <deque>
#include "enviro.h"

using namespace enviro;

/*In the Following state, the robot moves towards a pre-defined path that leads to the goal.
*/
class Following : public State, public AgentInterface {
        public:
        void entry(const Event& e) {
            // stop the robot if there's no next given location else get the next location
            if (paths.size() == 0){
                track_velocity(0,0,0,0);
            }else{
                std::pair<double, double> i = paths.front();
                std::cout << "x: " << i.first << ", y: " << i.second << std::endl;
                goal_x = i.first;
                goal_y = i.second;
                paths.pop_front();
            } 
        }
        void during() { 
            move_toward(goal_x, goal_y, 20, 10);
            if(std::fabs(position().x-goal_x) <=5 && std::fabs(position().y-goal_y)<=5 ){
                emit(Event("motion"));
            }
            if ( sensor_value(0) < 30 ) {
                emit(Event("inactive"));
            } 
        
        }
        void exit(const Event& e) {}
        void set_path(double x, double y) { 
            paths.push_back({x, y});
        }
        std::deque<std::pair<double, double>> paths;
        double goal_x;
        double goal_y;

    };

/* The Inactive state of robot is not moving
*/
class Inactive : public State, public AgentInterface {
    public:
    void entry(const Event& e) {label("stopped", 20, 5);}
    void during() { track_velocity(0,0,0,0);}              
    void exit(const Event& e) {clear_label();}     
};

/*The Restarting state of robot uses teleport to automatically 
move robot to the default start point.
*/
class Restart : public State, public AgentInterface {
    public:
    void entry(const Event& e) {track_velocity(0,0,0,0);}
    void during() { 
        teleport(-600, -280, 1.57);
        emit(Event("inactive"));
    }
    void exit(const Event& e) {}     
};

/* The Block Controller tries to link states and watches events
*/
class BlockController : public StateMachine, public AgentInterface {

    public:
    BlockController() : StateMachine(), AgentInterface() {
        set_initial(inactive);
        add_transition("motion", inactive, following);
        add_transition("motion", following, following);
        add_transition("startover", following, restart);
        add_transition("startover", inactive, restart);
        add_transition("inactive", restart, inactive);
        add_transition("inactive", following, inactive);
    }

    void init() {
        watch("sending_locations", [this](Event e){
            following.set_path(e.value()[0],e.value()[1]);
        });

        watch("button_click", [&](Event& e) {
            if(e.value()["value"] == "start_following" ){
                emit(Event("motion"));
            }
            if(e.value()["value"] == "restart" ){
                // emit the startover to activate restart state
                emit(Event("startover"));
                // emit another event catched by leading robot
                emit(Event("trigger_restart_button"));
                std::cout << "this line 81" << std::endl;
            }
            
            e.stop_propagation();
        });
        StateMachine::init();
    }

    void stop(){
        std::cout << "bc stop" << std::endl;
        StateMachine::stop();
    }

    Following following;
    Inactive inactive;
    Restart restart;

};

//This is the following robot
class Block : public Agent {
    public:
    Block(json spec, World& world) : Agent(spec, world) {
        add_process(bc);
    }
    private:
    BlockController bc;

};

DECLARE_INTERFACE(Block)

#endif