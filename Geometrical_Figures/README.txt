I used window_callback for changing the size of the window.
Key_callback for the input of the user 
T-Translate flag active,S-Scale,R-Rotate,O-Clear Screen.
1-square,2-hexagon,3-octagon.
ResetValues is to set the default parameters for scale,rotation and for difference for each one.Also here we find the random position functionality.
We take the inverse of perspective and view so that we transform the values between -1 1 to world coordinates.
We use perspective so that when we stretch the screen we see a square and not a paralepiped.
Then we check collision with screen borders,again we use model and multiply by the corners to see where it is in space and check with margins.
Then depending on the active flag we multiply by -1 to change its direction.

