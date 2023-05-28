#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "props.h"
#define verts_amount 11
#define IDC_BUTTON1 1
#define IDC_BUTTON2 2
#define IDC_BUTTON3 3
#define IDC_BUTTON4 4

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[] = "Lab 4 Mykhailo Kovalenko";

struct coordinates {
    double nx[verts_amount];
    double ny[verts_amount];
    double loopX[verts_amount];
    double loopY[verts_amount];
};

void arrow(double fi, double px, double py, HDC hdc) {
    double lx, ly, rx, ry;
    lx = px + 15 * cos(fi + 0.3);
    rx = px + 15 * cos(fi - 0.3);
    ly = py + 15 * sin(fi + 0.3);
    ry = py + 15 * sin(fi - 0.3);
    MoveToEx(hdc, lx, ly, NULL);
    LineTo(hdc, px, py);
    LineTo(hdc, rx, ry);
}

void drawUndirectedGraph(int centerX, int centerY, int radiusOfGraph, int radiusOfVertex, int radiusOfLoop, double angle,
                         struct coordinates coordinates, double **matrix,
                         HPEN KPen, HPEN GPen, HDC hdc) {
    for (int i = 0; i < verts_amount; ++i) {

        for (int j = 0; j < verts_amount; ++j) {
            MoveToEx(hdc, coordinates.nx[i], coordinates.ny[i], NULL);

            if (matrix[i][j] == 1) {
                if (i == j) {
                    SelectObject(hdc, GPen);
                    Ellipse(hdc, coordinates.loopX[i] - radiusOfLoop, coordinates.loopY[i] - radiusOfLoop,
                            coordinates.loopX[i] + radiusOfLoop, coordinates.loopY[i] + radiusOfLoop);
                    SelectObject(hdc, KPen);
                    //MoveToEx(hdc, coordinates.loopX[i], coordinates.loopY[i] + radiusOfLoop, NULL);
                } else {
                    LineTo(hdc, coordinates.nx[j], coordinates.ny[j]);
                }
            }
        }
    }
}

void drawArch(int startX, int startY, int finalX, int finalY, int archInterval, HDC hdc) {
    XFORM transformedMatrix;
    XFORM initialMatrix;
    GetWorldTransform(hdc, &initialMatrix);

    double angle = atan2(finalY - startY, finalX - startX) - M_PI / 2.0;
    transformedMatrix.eM11 = (FLOAT) cos(angle);
    transformedMatrix.eM12 = (FLOAT) sin(angle);
    transformedMatrix.eM21 = (FLOAT) (-sin(angle));
    transformedMatrix.eM22 = (FLOAT) cos(angle);
    transformedMatrix.eDx = (FLOAT) startX;
    transformedMatrix.eDy = (FLOAT) startY;
    SetWorldTransform(hdc, &transformedMatrix);

    const double archWidthRatio = 0.75;
    double archLength = sqrt((finalX - startX) * (finalX - startX) + (finalY - startY) * (finalY - startY));
    double radiusOfVertex = 15.0;
    double semiMinorAxis = archWidthRatio * archLength;
    double semiMajorAxis = archLength / 2;

    double ellipseStartY = semiMajorAxis;

    double vertexAreaSquared = semiMajorAxis * semiMajorAxis * radiusOfVertex * radiusOfVertex;
    double semiAxesSquared = semiMinorAxis * semiMinorAxis * semiMajorAxis * semiMajorAxis;
    double distanceFromCenter = semiMinorAxis * semiMinorAxis * ellipseStartY * ellipseStartY;
    double distanceFromVertex = semiMinorAxis * semiMinorAxis * radiusOfVertex * radiusOfVertex;
    double semiMinorAxisPow = pow(semiMinorAxis, 4);

    double intersection = semiMajorAxis *
                          sqrt(vertexAreaSquared - semiAxesSquared + distanceFromCenter - distanceFromVertex +
                               semiMinorAxisPow);
    double semiMinorAxisSquaredEllipseStartY = semiMinorAxis * semiMinorAxis * ellipseStartY;
    double denominator = -semiMajorAxis * semiMajorAxis + semiMinorAxis * semiMinorAxis;

    double contactYRightTop = (semiMinorAxisSquaredEllipseStartY - intersection) / denominator;
    double contactXRightTop = sqrt(radiusOfVertex * radiusOfVertex - contactYRightTop * contactYRightTop);
    double contactYBottom = archLength - contactYRightTop;
    double contactXLeftBottom = -contactXRightTop;

    if (archInterval <= verts_amount / 2) {
        Arc(hdc, -archWidthRatio * archLength, archLength, archWidthRatio * archLength, 0, 0, 0, 0, archLength);
        double angleOfArrow = -atan2(archLength - contactYBottom, contactXLeftBottom) + 0.3 / 3;
        arrow(angleOfArrow, contactXLeftBottom, contactYBottom, hdc);
    } else {
        Arc(hdc, -archWidthRatio * archLength, archLength, archWidthRatio * archLength, 0, 0, archLength, 0, 0);
        double angleOfArrow = -atan2(archLength - contactYBottom, -contactXLeftBottom) - 0.3 / 3;
        arrow(angleOfArrow, -contactXLeftBottom, contactYBottom, hdc);
    }

    SetWorldTransform(hdc, &initialMatrix);

}

void drawDirectedGraph(int n, int centerX, int centerY, int radiusOfGraph, int radiusOfVertex, int radiusOfLoop, double angle,
                       struct coordinates coordinates, double **matrix,
                       HPEN KPen, HPEN GPen, HDC hdc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            MoveToEx(hdc, coordinates.nx[i], coordinates.ny[i], NULL);
            if ((j >= i && matrix[i][j] == 1) || (j <= i && matrix[i][j] == 1 && matrix[j][i] == 0)) {
                if (i == j) {
                    SelectObject(hdc, GPen);

                    Ellipse(hdc, coordinates.loopX[i] - radiusOfLoop, coordinates.loopY[i] - radiusOfLoop,
                            coordinates.loopX[i] + radiusOfLoop, coordinates.loopY[i] + radiusOfLoop);


                    double radiusOfContact = radiusOfGraph + radiusOfLoop / 2.;
                    double triangleHeight = sqrt(3) * radiusOfVertex / 2.;
                    double loopAngle = atan2(triangleHeight, radiusOfContact);
                    double contactDistance = sqrt(radiusOfContact * radiusOfContact + triangleHeight * triangleHeight);
                    double angleToContactVertex = atan2(coordinates.ny[i] - centerY, coordinates.nx[i] - centerX);

                    double contactPointX = centerX + contactDistance * cos(angleToContactVertex + loopAngle);
                    double contactPointY = centerY + contactDistance * sin(angleToContactVertex + loopAngle);

                    double curvatureAngle = angleToContactVertex + 0.3 / 2.;
                    arrow(curvatureAngle, contactPointX, contactPointY, hdc);
                    SelectObject(hdc, KPen);
                } else {
                    LineTo(hdc, coordinates.nx[j], coordinates.ny[j]);
                    double line_angle = atan2(coordinates.ny[i] - coordinates.ny[j], coordinates.nx[i] - coordinates.nx[j]);
                    arrow(line_angle, coordinates.nx[j] + radiusOfVertex * cos(line_angle),
                          coordinates.ny[j] + radiusOfVertex * sin(line_angle), hdc);
                }
            } else if (j < i && matrix[i][j] == 1 && matrix[j][i] == 1) {
                drawArch(coordinates.nx[i], coordinates.ny[i], coordinates.nx[j], coordinates.ny[j], fabs(i - j), hdc);
            }

        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
    WNDCLASS w;
    w.lpszClassName = ProgName;
    w.hInstance = hInstance;
    w.lpfnWndProc = WndProc;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hIcon = 0;
    w.lpszMenuName = 0;
    w.hbrBackground = WHITE_BRUSH;
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    if (!RegisterClass(&w)) {
        return 0;
    }
    HWND hWnd;
    MSG lpMsg;
    hWnd = CreateWindow(ProgName,
                        (LPCSTR) "Lab3 (Mykhailo Kovalenko IM-22)",
                        WS_OVERLAPPEDWINDOW,
                        100,
                        100,
                        950,
                        800,
                        (HWND) NULL,
                        (HMENU) NULL,
                        (HINSTANCE) hInstance,
                        (HINSTANCE) NULL);
    ShowWindow(hWnd, nCmdShow);
    while (GetMessage(&lpMsg, hWnd, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return (lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    HWND Button_directed;
    HWND Button_undirected;
    HWND Button_modified;
    int state = 0;
    switch (messg) {
        case WM_CREATE: {
            Button_directed = CreateWindow(
                    (LPCSTR) "BUTTON",
                    (LPCSTR) "Switch to Directed",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    700,
                    20,
                    160,
                    50,
                    hWnd,
                    (HMENU) IDC_BUTTON1,
                    (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                    NULL);
            Button_undirected = CreateWindow(
                    (LPCSTR) "BUTTON",
                    (LPCSTR) "Switch to Undirected",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    700,
                    100,
                    160,
                    50,
                    hWnd,
                    (HMENU) IDC_BUTTON2,
                    (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                    NULL);
            Button_modified = CreateWindow(
                    (LPCSTR) "BUTTON",
                    (LPCSTR) "Switch to Modified",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    700,
                    180,
                    160,
                    50,
                    hWnd,
                    (HMENU) IDC_BUTTON3,
                    (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                    NULL);
            Button_modified = CreateWindow(
                    (LPCSTR) "BUTTON",
                    (LPCSTR) "Switch to Condensation",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    700,
                    260,
                    160,
                    50,
                    hWnd,
                    (HMENU) IDC_BUTTON4,
                    (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                    NULL);
            return 0;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {

                case IDC_BUTTON1:
                    state = 0;
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;

                case IDC_BUTTON2:
                    state = 1;
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
                case IDC_BUTTON3:
                    state = 2;
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
                case IDC_BUTTON4:
                    state = 3;
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
            }
        }
        case WM_PAINT :
            hdc = BeginPaint(hWnd, &ps);

            SetGraphicsMode(hdc, GM_ADVANCED);
            HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
            HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
            HPEN GPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
            HPEN NoPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
            SelectObject(hdc, NoPen);
            Rectangle(hdc, 0, 0, 670, 700);


            char *nn[verts_amount] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10\0", "11\0"};

            struct coordinates coordinates;

            double circleRadius = 200;
            double vertexRadius = circleRadius / 11;

            double loopRadius = vertexRadius;
            double dtx = vertexRadius / 2.5;

            double circleCenterX = 370;
            double circleCenterY = 360;


            double angleAlpha = 2.0 * M_PI / (double) verts_amount;
            for (int i = 0; i < verts_amount; i++) {

                double sinAlpha = sin(angleAlpha * (double) i);
                double cosAlpha = cos(angleAlpha * (double) i);
                coordinates.nx[i] = circleCenterX + circleRadius * sinAlpha;
                coordinates.ny[i] = circleCenterY - circleRadius * cosAlpha;
                coordinates.loopX[i] = circleCenterX + (circleRadius + loopRadius) * sinAlpha;
                coordinates.loopY[i] = circleCenterY - (circleRadius + loopRadius) * cosAlpha;

            }


            double **T = randm(verts_amount);
            double coefficient = 1.0 - 0.01 - 0.01 - 0.3;
            double **A = mulmr(coefficient, T, verts_amount);

            double **R = randm(verts_amount);
            double **C = symmetricalMatrix(mulmr(coefficient, R, verts_amount), verts_amount);

            double** K = randm(verts_amount);
            double modifiedCoefficient = 1.0 - 0.005 - 0.005 - 0.27;
            double** D = mulmr(modifiedCoefficient, K, verts_amount);

            double **condensationMatrix = condensationMatrixWithCoefficient(modifiedCoefficient);
            double **matrix = generateAdjacencyMatrixFromStrongComponents(condensationMatrix);
            double **reachabilityMatrix = getReachabilityMatrix(D);
            double **connectivityMatrix = getStrongConnectivityMatrix(reachabilityMatrix);
            int amount = getStrongComponents(connectivityMatrix);
            printf("%d", amount);

            SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
            SelectObject(hdc, KPen);

            if (state == 0) {
                drawDirectedGraph(verts_amount,circleCenterX, circleCenterY, circleRadius, vertexRadius, loopRadius, angleAlpha,
                                  coordinates, A, KPen, GPen, hdc);
            }
            if (state == 1){
                drawUndirectedGraph(circleCenterX, circleCenterY, circleRadius, vertexRadius, loopRadius, angleAlpha,
                                    coordinates, C, KPen, GPen, hdc);
            }
            if (state == 2) {
                drawDirectedGraph(verts_amount,circleCenterX, circleCenterY, circleRadius, vertexRadius, loopRadius, angleAlpha,
                                    coordinates, D, KPen, GPen, hdc);
            }
            if (state == 3) {
                drawDirectedGraph(amount,circleCenterX, circleCenterY, circleRadius, vertexRadius, loopRadius, angleAlpha,
                                    coordinates, matrix, KPen, GPen, hdc);
            }

            SelectObject(hdc, BPen);
            SelectObject(hdc, GetStockObject(DC_BRUSH));
            SetDCBrushColor(hdc, RGB(204, 204, 255));
            SetBkMode(hdc, TRANSPARENT);
            int length = state == 3 ? amount : verts_amount;

            for (int i = 0; i < length; ++i) {
                Ellipse(hdc, coordinates.nx[i] - vertexRadius, coordinates.ny[i] - vertexRadius,
                        coordinates.nx[i] + vertexRadius, coordinates.ny[i] + vertexRadius);
                TextOut(hdc, coordinates.nx[i] - dtx, coordinates.ny[i] - vertexRadius / 2, nn[i], 2);
            }


            EndPaint(hWnd, &ps);

            freeMatrix(A, verts_amount);
            freeMatrix(C, verts_amount);
            freeMatrix(D, verts_amount);
            freeMatrix(matrix, verts_amount);
            freeMatrix(condensationMatrix, verts_amount);
            freeMatrix(connectivityMatrix, verts_amount);
            freeMatrix(reachabilityMatrix, verts_amount);
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return (DefWindowProc(hWnd, messg, wParam, lParam));
    }
    return 0;
}
