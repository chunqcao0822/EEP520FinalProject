# Software Engineering For Embedded Applications Final Project

### Goals :

1. First Part: A user-controlled leading robot that requires the user to guide it by clicking on the screen. The robot is expected to navigate through a static maze box that has already been generated. If the robot detects an obstacle in front of it, a label will appear on the robot's icon to prompt the user to reclick the screen and redirect the robot to avoid the obstacle. The starting point is pre-defined, but the user has the freedom to decide where they want the robot to go. The endpoint will be the final point clicked by the user on the screen.

2. Second Part: Once the user has guided the leading robot to traverse a certain path, a follower robot (represented by a square) will traverse the same path without the need for further interaction from the user. The leading robot will remember the path and send the positions to the follower robot and then it will automatically follow the same path. The follower robot is equipped with a sensor that will detect obstacles (including the other robot or walls) and stop automatically if any obstacles are encountered. Additionally, once the follower robot catches up to the leading robot, it will also stop. 

### Key Challenges and Solutions:

1. It can be difficult to implement event watching by emitting events and communicating with various processes or state machines. However, creating a flowchart to visualize the processes and understand how they work can assist in figuring out the implementation process.

2. Creating a maze and allocating blocks within it can be a challenging task. However, one helpful approach is to draw the maze in a corresponding matrix. This can simplify the process and assist in accurately allocating blocks within the maze.

3. Coordinating two robots using two processes can be a difficult task since they require the ability to communicate with each other without disrupting their individual ongoing processes. The Event class can help two StateMachine interact each other. 

4. Trainsiting from one state to another in StateMachine should be very careful. It's unneccesary to add lots of useless transtions. At the same time, I also want to make sure that each state can be entered smoothly, while also avoiding the robots getting stuck in one state and being unable to exit.

5. Calliberating the speed, force and angular velocity took times. Since the robots are self-guided and user cannot control them all the time so robot can move smoothly by itself without over-rotating and stop accordingly.

6. To click one button and teleport two robots to their default position is hard. It's not able to install two button click event into two process. The solution would be that one process watch the button click event and send a new event to another process.

7. From all above, the biggest challenge is to learn C++, Elma, and Enviro. To overcome this challenge, it is essential to thoroughly understand all the course material and related documentation. Dedication, practice, and seeking help when needed can also assist in mastering these tools.

### Install and Run

1. Clone the project repo

    ```
    git clone https://github.com/chunqcao0822/EEP520FinalProject.git

    cd project
    ```

2. Build the docker:

    ```
    docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.4 bash
    ```

3. Run the code
    
    Uses the enviro setup manager to start a web server from which your browser can access the ENVRIO client

    ```
    esm start 
    make
    enviro
    ```

    open http://localhost from browser, enjoy playing and have fun.


### Run the Project
To move the leading robot (yellow), you can click the screen to guide the robot move toward that location. The start point will always be at the left upper cornor of the maze. After you guide the leading robot, click the "Start Following" button at the upper right cornor of the screen. Once clicking, the follower robot (red square) will follow the same path as the leading robot guided by user. If the follower robot can not pass through the obstacles, you can click the "Restart" button. Then the leading and following robots will automatically be moved to their start point. 

### Demo
![](/misc/demo.gif)


### Acknowedges
- Professor Makhsous's Github repo: https://github.com/sosper30/eep520
- Professor Klavins Github repo:  https://github.com/klavinslab/enviro
- Thank Professor Makhsous teaching this course and all the well-documented course material.
- Thank our TA, Sanskar Naik, for answering all the questions regarding the final project.



### License
This software is open source and uses the MIT license. Details can be found [here](https://opensource.org/license/mit/)
