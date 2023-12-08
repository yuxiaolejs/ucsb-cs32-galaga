# UCSB-CS32-GALAGA
## Group Members
- Andy J.
- Luis B.
- Luxiang Z.
- Tianle Y.

## UCSB CS32 GALAGA

An OpenGL replica of galaga with simplified logic / graphics

## Update Notes
### Score System!
Wonder how you are doing in the game? We got you! Just check your score and show it off to your friends!
### Leader board!
Even better than just a score! Now you can have your name on leader board if you have top 25 scores!
This leader board is based on the username that your computer is logged into while running the game and your
ip address. Don't be suprised by people having multiple names on it! They just got more instances of themselves (ip addresses).

[Check out our leader board here](https://yuxiaolejs.github.io/ucsb-cs32-galaga/leaderboard.html)
### Smart Projectile!
Getting smart! You will be chased by smart projectiles! Don't get hit by them if you can, they will disappear in 6 seconds after being launched.
### HealthPoint System!
Now you have more than one lives, kill enermy to earn more! Most importantly, don't crash into other ships! You will lose a lot of HP by doing that!
### Ending Screen!
Yes yes! You won't be thrown out of the game immediatly any more! You will see an ending screen after dying, which not only shows information about this project, but also provides a leader board!

## General Documentation
### System Requirements
- Platform: Any that supports OpenGL, FreeGLUT, gcc, cmake
- OS: Linux recommanded, but any system should be able to run.
- CPU: 2 Core or more
- Memory: 4 GB or more
- Disk: 100 MiB or more
- Software


  | name     | version |
  | -------- | ------- |
  | CMake    | ^3.14   |
  | OpenGL   | 4.1     |
  | FreeGLUT | ^3.4.0  |
  | libpng   | -       |
  | curlpp   | -       |
  | jsoncpp  | -       |


### Compiling the program
```
cd ucsb-cs32-galaga # Get into the git repo, don't do if you are not using git
cmake .             # Please do this first, or you won't have a makefile
make                # I assume everyone knows what this means...
```
### Running the program
```
./ezg
```

### How to play
- Use `w, s, a, d` to move
- Avoid enermy's projectile
- Shoot enermy with projectile
- Destroy enermy ships to recover hp
- Prevent losing all hp

## Technical Details
This is technical details for TA to review.
### Game implimentation
- [Game engine design & use (English)](engine.md)
- [Game engine design & use (Chinese)](engine-cn.md)
### Checkpoint info
- [Check point 1](ckpt1.md)
- [Check point 2](ckpt2.md)
- [Check point 3](ckpt3.md)