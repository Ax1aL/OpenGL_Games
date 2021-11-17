Mouse_button_callback is for activating the flag for reseting the game.
ProccessInput is for camera movement.
CheckCollision is for checking if the ball is hiting the cylinders.We check that all 3 axes collide .We make intervals for each one,depending on the center, radius and heigth.
resetStage is for setting everything back,camera,pins,ball etc.
We make the sphere using sin and cos,get each point position.
For cylinder we make 2 circles for top and bottom and then we unite them with triangles.
We get the rows needed and with some mathematics dependign on the row and parity of pins we set them in place.We store the results.
We can't move camera until the ball is launched,it can be moved using the mouse position if is around the ball,and once launched we can move the camera around.
We launch the ball with Mouse LEFT it starts spining and moving towards the cylinders and check collision with them.
If we press Mouse RIGHT- we reset the stage or if the ball gets to far we reset it again.After reset the score is being displayed.
Try find out easter eggs ;).
