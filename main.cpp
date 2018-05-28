#include <QCoreApplication>
#include <qmath.h>
//#include <QVector>
#include <vector>

const double EARTH_RADIUS = 6371009;

struct LatLng{
  double Latitude;
  double Longitude;
};

double ToRadians(double input)
{
    return input / 180.0 * M_PI;
}

double PolarTriangleArea(double tan1, double lng1, double tan2, double lng2)
{
    double deltaLng = lng1 - lng2;
    double t = tan1 * tan2;
    return 2 * qAtan2(t * qSin(deltaLng), 1 + t * qCos(deltaLng));
}

double ComputeSignedAreaRad(std::vector<LatLng> path, double radius)
{
    int size = path.size();
    if (size < 3) { return 0; }
    double total = 0;
    LatLng prev = path[size - 1];
    double prevTanLat = qTan((M_PI / 2 - ToRadians(prev.Latitude)) / 2);
    double prevLng = ToRadians(prev.Longitude);

    foreach (LatLng point, path)
    {
        double tanLat = qTan((M_PI / 2 - ToRadians(point.Latitude)) / 2);
        double lng = ToRadians(point.Longitude);
        total += PolarTriangleArea(tanLat, lng, prevTanLat, prevLng);
        prevTanLat = tanLat;
        prevLng = lng;
    }
    return total * (radius * radius);
}

double ComputeSignedArea(std::vector<LatLng> path)
{
    return ComputeSignedAreaRad(path, EARTH_RADIUS);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LatLng p0 = {
        39.986089,
        116.326865
    };

    LatLng p1 = {
        39.986098,
        116.327318
    };

    LatLng p2 = {
        39.986780,
        116.327290
    };

    LatLng p3 = {
        39.986778,
        116.326860
    };

    std::vector<LatLng> b;
    b.push_back(p0);
    b.push_back(p1);
    b.push_back(p2);
    b.push_back(p3);

    double area = ComputeSignedArea(b);

    return a.exec();
}
