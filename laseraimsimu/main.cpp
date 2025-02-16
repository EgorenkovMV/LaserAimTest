#include <iostream>
#include <cstdlib>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace {
int WIDTH = 1000;
int HEIGHT = 1000;
int FPS = 30;
int DURATION = 60;    // seconds
int SPEC_RADIUS = 50;
int ROAM_DISTANCE = 100;
uchar BRIGHTNESS_LOW = 25;
uchar BRIGHTNESS_LOW_SPREAD = 25;
uchar BRIGHTNESS_HIGH = 230;
uchar BRIGHTNESS_HIGH_SPREAD = 25;
string FILEPATH = "../../../laserAimSimu.avi";
}

unsigned char generateBrightnessLow()
{
    return BRIGHTNESS_LOW + (static_cast<uchar>(rand()) % (2 * BRIGHTNESS_LOW_SPREAD) - BRIGHTNESS_LOW_SPREAD);
}

unsigned char generateBrightnessHigh()
{
    return BRIGHTNESS_HIGH + (static_cast<uchar>(rand()) % (2 * BRIGHTNESS_HIGH_SPREAD) - BRIGHTNESS_HIGH_SPREAD);
}

int main()
{
    Size size {WIDTH, HEIGHT};
    // lossless codecs: FFMPEG FFV1, Huffman HFYU, Lagarith LAGS, etc...
    VideoWriter output(FILEPATH, VideoWriter::fourcc('F', 'F', 'V', '1'), FPS, size, 0);

    if (!output.isOpened()) {
        cout << "Unable to find a codec" << endl;
        return 1;
    }

    cout << "Started. It may take some time..." << endl;

    int specX = WIDTH / 2;
    int specY = HEIGHT / 2;

    for (int i = 0; i < FPS * DURATION; i++) {
        Mat frame {size, CV_8UC1};

        int roamX = rand() % (2 * ROAM_DISTANCE) - ROAM_DISTANCE;
        int roamY = rand() % (2 * ROAM_DISTANCE) - ROAM_DISTANCE;

        if (specX + roamX >= WIDTH - SPEC_RADIUS or specX + roamX <= SPEC_RADIUS)
        {
            specX -= roamX;
        }
        else {
            specX += roamX;
        }

        if (specY + roamY >= WIDTH - SPEC_RADIUS or specY + roamY <= SPEC_RADIUS)
        {
            specY -= roamY;
        }
        else {
            specY += roamY;
        }

        frame.forEach<uchar>([&specX, &specY](uchar &p, const int *position) -> void {
            int distSquared = pow(position[0] - specX, 2) + pow(position[1] - specY, 2);
            if (distSquared < pow(SPEC_RADIUS, 2)) {
                p = generateBrightnessHigh();
            }
            else {
                p = generateBrightnessLow();
            }
        });


        output.write(frame);

        if (i % FPS == 0) {
            cout << "Working on " << i << "-th frame" << endl;
        }
    }


    output.release();
    destroyAllWindows();
    cout << "Done." << endl;
    return 0;
}
