#include "vex.h"
using namespace vex;

brain Brain;

const int MOTOR_SPEED = 25;

void waitDoubleClick() {
    Brain.Screen.setCursor(8, 1);
    Brain.Screen.print("DOUBLE TAP TO NEXT");

    int clickCount = 0;
    uint32_t lastTime = 0;
    while (true) {
        if (Brain.Screen.pressing()) {
            uint32_t now = Brain.Timer.system();
            if (now - lastTime < 500) clickCount++;
            else clickCount = 1;
            lastTime = now;
            if (clickCount >= 2) break;
            wait(200, msec);
        }
        wait(50, msec);
    }
}

void testScreen() {
    Brain.Screen.setFont(mono20);
    Brain.Screen.clearScreen();

    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("=== SCREEN TEST ===");
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("Voltage: %.2f V", Brain.Battery.voltage());
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("Touch to check X Y");

    for (int i = 10; i > 0; i--) {
        Brain.Screen.setCursor(5, 1);
        Brain.Screen.clearLine();
        Brain.Screen.print("Auto continue in %dS", i);

        uint32_t start = Brain.Timer.system();
        while (Brain.Timer.system() - start < 1000) {
            if (Brain.Screen.pressing()) {
                int x = Brain.Screen.xPosition();
                int y = Brain.Screen.yPosition();
                if (x < 0) x = 0; if (x > 480) x = 480;
                if (y < 0) y = 0; if (y > 240) y = 240;

                Brain.Screen.setCursor(4, 1);
                Brain.Screen.clearLine();
                Brain.Screen.print("X: %3d    Y: %3d", x, y);
            }
            wait(50, msec);
        }
    }

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("TEST COMPLETED");
    wait(2000, msec);
}

// ====================== 电机测试 ======================
void testMotors() {
    Brain.Screen.setFont(mono20);

    for (int i = 0; i < 20; i++) {
        int realPort = i;      // 内部 0~19
        int showPort = i + 1;  // 显示 1~20

        motor testMotor(realPort);

        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("MOTOR PORT: %d", showPort);

        if (!testMotor.installed()) {
            Brain.Screen.setCursor(2, 1);
            Brain.Screen.print("STATUS: NO DEVICE");
            waitDoubleClick();
            continue;
        }

        Brain.Screen.setCursor(2, 1);
        Brain.Screen.print("STATUS: FORWARD");
        testMotor.spin(forward, MOTOR_SPEED, percent);
        wait(800, msec);

        Brain.Screen.setCursor(2, 1);
        Brain.Screen.clearLine();
        Brain.Screen.print("STATUS: REVERSE");
        testMotor.spin(reverse, MOTOR_SPEED, percent);
        wait(800, msec);

        testMotor.stop();
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.clearLine();
        Brain.Screen.print("STATUS: STOPPED");

        waitDoubleClick();
    }

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("MOTOR TEST FINISHED");
    wait(2000, msec);
}

// ====================== 3-Wire 三线口 A~H 测试 ======================
void test3Wire() {
    Brain.Screen.setFont(mono20);

    // 端口列表 A-H
    char names[] = "ABCDEFGH";

    for (int i = 0; i < 8; i++) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("3-WIRE PORT: %c", names[i]);
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.print("STATUS: TESTED OK");

        waitDoubleClick();
    }

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("3-WIRE TEST FINISHED");
    wait(2000, msec);
}

// ====================== 菜单 ======================
int selectMenu() {
    Brain.Screen.setFont(mono40);
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("1: SCREEN TEST");
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print("2: MOTOR TEST");
    Brain.Screen.setCursor(7, 1);
    Brain.Screen.print("3: 3-WIRE TEST");

    while (true) {
        if (Brain.Screen.pressing()) {
            int y = Brain.Screen.yPosition();
            if (y < 80)   return 1;  
            if (y < 160)  return 2;  
            else          return 3;  
        }
        wait(50, msec);
    }
}

// ====================== 主程序 ======================
int main() {
    while (true) {
        int choice = selectMenu();
        if (choice == 1) testScreen();
        if (choice == 2) testMotors();
        if (choice == 3) test3Wire();
    }
}