#define WIDTH 1440.0f
#define HEIGHT 900.0f

#define ZOOM_DEFAULT 1.0f
#define ZOOM_MAX 8.0f
#define ZOOM_MIN 0.5f

#define X_MAX_DEFAULT 3.0f
#define X_MIN_DEFAULT -3.0f
#define Y_MAX_DEFAULT (X_MAX_DEFAULT * (HEIGHT / WIDTH))
#define Y_MIN_DEFAULT (X_MIN_DEFAULT * (HEIGHT / WIDTH))
#define X_SPACING 0.1f

#define SAMPLE_COUNT ((int)((X_MAX_DEFAULT - X_MIN_DEFAULT) / X_SPACING))

#define MAP(a, x1, x2, y1, y2) ((((y2) - (y1)) / ((x2) - (x1))) * ((a) - (x1)) + y1)
#define LEN(array) (sizeof((array)) / sizeof((array)[0]))