# EMBSYS 330 Final Project
## Objective
Run through the (almost) complete project life cycle:
1. Requirements Analysis
2. Architecture / High Level Design
4. Low Level Design
5. Construction
6. Test

The learner shall chose a project that incorporates the touch screen and multiple sensors on the Discovery board and integrate them into a working project. The nature of the project is up to the student.

# Check Out Project From GitHub Classroom

Check out the GitHub Classroom project to the `C:\Users\<username>\zephyrproject\` directory so that it ends up with a directory name similar to:

`C:\Users\<username>\zephyrproject\embsys-330-sp2x-final-project-<github_username>`
Where `<username>` is your windows username.

On Mac or Linux extract to the `~\zephyrproject\` directory so it ends up with a directory name similar to `~\zephyrproject\embsys-330-sp2x-final-project-<github_username>`

NOTE: GitHub Classroom modifies the name of the folder based on the class name and appends your GitHub username to the repository name, which will be reflected in the directory name, e.g., `embsys-330-sp2x-final-project-<github_username>`. This is expected.

## Project branches
Create a working branch named `final_project`: `git checkout -b final_project`

For each milestone create a separate working branch based off of the `final_project` branch rather than `main`. These branches will have the same name as the primary document in the `documentation` directory.

1. `project_idea`
2. `software_requirements`
3. `architecture`
4. `detailed_design`

Once the documentation is created for each milestone, generate a pull request to merge it back into `final_project` rather than into `main`. This PR can be submitted for ungraded review in the appropriate milestone assignment.

Once the project is complete, merge all the documentation branches into `final_project` and create a PR to merge `final_project` into `main`.

## Project Flow
1. Choose a project
2. Begin Requirements Capture
3. Start Architecture / High Level Design
    1. Design the software with testing  in mind
4. Identify areas requiring low level design
5. Start low level design for the identified areas
6. Construct the software, updating design documents as necessary
    1. Construct the software with testing in mind. You can use test-driven development or add tests afterwards
7. Test the software using Twister and the Ztest framework built into Zephyr
8. Demonstrate the project in Week 10, either live or by prerecorded demo

## Information Delivery
An introduction to software engineering will be given in Module 01. Additional resources on Software Engineering are available through the UW library and online. 

Modules 02 and 03 cover Hierarchical State Machines and Statecharts, as well as Zephyr's State Machine Framework. You may chose to use HSMs in your project design or use traditional well-formulated embedded design concepts. The PSICC2 book in the files section of Canvas will be a valuable resource.

Module 04 covers Test Driven Development, Unit Testing and the Zephyr Ztest framework and Twister test runner. You do not have to practice TDD for this project, but you do need to provide unit tests and integration tests for the project.

Assignments 03 through 07 provide an introduction to Zephyr application design, the State Machine Framework, multi-threading and interrupts.

## Project Timing
The only hard deadline is the entire project and all deliverables must be completed by midnight on the day of the final lecture. As with the previous semesters, UW expects grades to be submitted quickly, precluding any extensions for this project.

That being said, there are several soft milestones for delivering Requirements, Architecture/High-level Design and Low-level Design. You may use these ungraded opportunities to discuss your project progress with the instructor and gain feedback if you desire, but no points will be lost for not doing so.

## Project Requirements
1. Demonstrate an understanding of embedded systems design through documents and/or diagrams
    1. This must include requirements, architecture / high level design and detailed design and may include UI design, state charts, activity diagrams and other relevant documentation
2. Demonstrate control over multiple sensors
    1. The sensors should use their interrupts (if available) to notify the application of new data
3. Demonstrate a knowledge of multi-threaded system design
4. MVP: Demonstrate use of the display for data output
    1. Optionally demonstrate use of the touchscreen to create a GUI
5. Demonstrate adequate test coverage of the code, including unit tests of modules and integration tests running on the hardware
    1. Note that since this course was developed for Windows, native-sim is not available and tests are set up to be performed on the development board.
    2. If you want to test high-level modules using QEMU you are free to make a separate test directory for that in addition to whatever on-device testing you have in the `tests` directory.
6. Document the operation of the application in the embsys330_final_project_readme.md file

## Project Framework
The project framework is derived from the LVGL demo at `zephyr\samples\subsys\display\lvgl\`

The demo is placed in the `app` sub-directory rather than in the folder root as before to allow for a separate `tests` directory.

Changes made to the LVGL demo:
1. Added a custom overlay and config file in the boards sub-directory to define SRAM2 and tell LVGM to use a custom section for the video memory
1. Set the video buffer size (`CONFIG_LV_Z_VDB_SIZE`) to 14% of the screen size, instead of the default 64%, since that is the maximum size that fits in SRAM2
1. Added a custom linker script to put the LVGL video buffer in SRAM2
1. Added the touchscreen shield to CMakeLists.txt so it did not have to be defined on the command line
1. `CONFIG_DEBUG_THREAD_INFO` and `CONFIG_THREAD_NAME` set in `prj.conf` to enable RTOS debug information
1. Added `count.c` and `count.h` to create a simple module to demonstrate testing.
1. Renamed the original demo readme to `lvgm_demo_README.rst`

### Memory Usage
LVGM uses a lot of memory.

There is 16KB assigned for a heap for dynamic allocation of UI objects, which can be modified by changing `CONFIG_LV_Z_MEM_POOL_SIZE`.

The video buffer for rendering images before transferring them to the screen is defined as a percentage of screen size: `CONFIG_LV_Z_VDB_SIZE`. The default is 64% of the 225KB screen memory, which would be 144KB - larger than all available RAM on the STM52L475. The MCU has two SRAM banks of 96KB and 32KB respectively, and we are using the entirety of the latter to store the video buffer, (14% screen size). Note this implies that objects containing more than 10,752 pixels will not fit in the video buffer.

Documentation and simulators for the LVGL graphics library are available at: https://lvgl.io/

### Hardware
You must use the LCD screen. The project is set up to use it. You may optionally use the touchscreen sensor, which is also set up for use.

You may use the Music Maker board. Be aware there is no Zephyr driver provided for it.

You may use any sensor, peripheral or networking component on the development board. Not all peripherals have Zephyr drivers available for them.

You may add hardware to your system if you have it available or wish to purchase it with your own funds e.g. other Arduino shields, motor controllers, sensors, etc. However, you cannot buy your way into extra credit - doing something novel with the kit provided will be worth as much as doing something novel with additional hardware.

**Important Note:** LED1 shares a GPIO pin with SPI1_SCK. You cannot use LED1 without disabling part of the SPI to the touchscreen and causing lockup of the SPI driver. LED2 is on its own line, and can be used.

### Testing
Tests are store in the `tests` sub-directory and can be run with the following commands in PowerShell which will run all tests defined in `tests\testcase.yaml`:

```
cd C:\Users\<username>\zephyrproject\
.\.venv\Scripts\Activate.ps1
cd embsys330_final_project\tests
west twister --device-testing --device-serial COM3 --device-serial-baud 115200 --no-detailed-test-id --short-build-path -p disco_l475_iot1 -T .
```
Note the period at the end of the twister command, and substitute `COM3` for the appropriate COM port on your setup.

### Zephyr Issue(s)
#### Touch Controller I2C bus
The I2C bus for the touch controller is not assigned correctly.

Go to `zephyr\boards\st\disco_l475_iot1\arduino_r3_connector.dtsi` and change the line
```
arduino_i2c: &i2c3 {};
```
To:
```
arduino_i2c: &i2c1 {};
```

#### Redefinition of TZNAME_MAX
When testing you may encounter build failures caused by redefinition of `TZNAME_MAX`. This is a bug that was fixed in Zephyr 4.0.0, but we are using 3.7.0.

To remedy the issue, you need to modify the zephyr source code by wrapping the (re)definition of TZNAME_MAX with a guard condition as shown [here](https://github.com/zephyrproject-rtos/zephyr/commit/5ad762f):

This should be found near line 331 of `zephyr\include\zephyr\posix\posix_features.h` and the definition should be replaced with the following three lines:

```
#ifndef TZNAME_MAX
#define TZNAME_MAX          _POSIX_TZNAME_MAX
#endif
```

### twister-out Directories
In testing you will generate a lot of directories named `twister-out`, `twister-out.1` etc. in the `zephyrproject\embsys_330_final_project\tests` directory.

Windows is very slow at deleting these folders through file explorer. Options to speed it up include the following CLI commands run from the `tests` directory:

1. Powershell: `Remove-Item -Path twister-out* -Recurse -Force`
2. Git Bash: `rm -rf twister-out*`


# Deliverables
## Design Documentation

Documentation shall be in Markdown format in the `documentation` directory of the project file. Submit documents as pull requests, and after approval merge them into `main`. Blank documents are in the `documentation` directory ready to fill in, but feel free to add your own if required to complete your design.

For diagrams, feel free to use any diagram generation tool you feel comfortable with (Inkscape, Visio, UMLet, LucidChart, etc.) but submit them as PNG files in the `documents\images` directory, linked to your markdown files.

At minimum the following is expected:
1. Project Idea
2. Problem Definition - a breakdown of the project idea into a more concrete description of how the project will work. This may include what hardware to use, and user stories or other means of describing the operation
3. Requirements document
4. Architecture / High Level Design
    1. This should be natural language with appropriate diagrams
    2. A layer diagram is not an acceptable architecture diagram. We know you're building an application on top of Zephyr OS services, on top of the Zephyr kernel, on top of the device drivers, etc.
5. Detailed design of appropriate parts of the system
    1. This should be natural language with appropriate diagrams

## Code Deliverables
1. Project code:
    1. Code shall match the design documentation
    2. Code shall be well constructed (See Code Complete for tips on this)
    3. Code shall adhere to the Zephyr Coding Style as described here: https://docs.zephyrproject.org/3.7.0/contribute/guidelines.html#coding-style
    4. Code should be appropriately documented. i.e. document design decisions, not `x++; /* increment x */`
2. Test code:
    1. `test_counter` shows an example of testing code from the application.
    2. All module interfaces should be tested
    3. Low level hardware CAN be tested, since we are testing on the development board, but is not a hard requirement

## Submission
1. Commit your working code to your Git repository and push to GitHub
2. In GitHub go to the green "<> Code" button, click on the down arrow and select "Dowload Zip"
    1. Name the zip file `embsys330_final_project_<lastname>_<firstname>.zip`
3. Create a GitHub pull request in your repository
4. Submit both the URL of your pull request and the Zip file to Canvas
	1. When submitting the URL, put it on a single line with no text before or after so it's easy to double-click on the text and 'open in new tab'

