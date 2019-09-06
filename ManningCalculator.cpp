#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

using namespace std;

void constantRoughnessDepthCalculator(double &, double &, double &, double &, double &, double &, double &, double &, double &, double &, double &);
void changingRoughnessDepthCalculator(double &, double &, double &, double &, double &, double &, double &, double &, double &, double &, double &, double &);
double calcFlow(double, double, double, double);
double calcArea(double, double);
double calcArea1(double, double);
double calcHydraulicRadius(double, double);
double calcWettedPerimeter(double, double);
double calcWettedPerimeter1(double, double);
double calcTheta(double, double);
double calcVelocity(double, double);
double calcFrictionCoefficient(double, double, double);
void output1(double, double, double, double, double, double, double, double);
void output2(double, double, double, double, double, double, double, double, double);
void input(double&,  double&, double&, double&);
char select();

int main(){
    while(true){
        char check = select();
        double diameter, slope, desiredFlow, roughness, fpRoughness, depthOfFlow = .5, radius,
               theta, area, wettedPerimeter, hydraulicRadius, frictionCoefficient, flow, velocity;
        switch(check){
            case '1':
                input(diameter,  fpRoughness, slope, desiredFlow);
                radius = diameter / 2;
                frictionCoefficient = fpRoughness;
                constantRoughnessDepthCalculator(theta, area, wettedPerimeter, hydraulicRadius, flow, velocity, depthOfFlow, radius, frictionCoefficient, slope, desiredFlow);
                output1(theta, area, wettedPerimeter, hydraulicRadius, flow, velocity, depthOfFlow, radius);
                break;
            case '2':
                input(diameter,  fpRoughness, slope, desiredFlow);
                radius = diameter / 2;
                changingRoughnessDepthCalculator(theta, area, wettedPerimeter, hydraulicRadius, flow, velocity, depthOfFlow, radius, frictionCoefficient, slope, desiredFlow, fpRoughness);
                output2(theta, area, wettedPerimeter, hydraulicRadius, flow, velocity, depthOfFlow, radius, frictionCoefficient);
                break;
            default:
                return 0;
        }
    }
}

void constantRoughnessDepthCalculator(double &theta, double &area, double &wettedPerimeter, double &hydraulicRadius, double &flow, double &velocity, double &depthOfFlow, double &radius,  double &frictionCoefficient, double &slope, double &desiredFlow){
    double incr = .5;
    theta = calcTheta(radius, depthOfFlow);
    area = calcArea(radius, theta);
    wettedPerimeter = calcWettedPerimeter(radius, theta);
    hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
    flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
    velocity = calcVelocity(flow, area);
    while(incr != 0){
        incr /= 2;
        if(flow > desiredFlow + 1e-6){
            depthOfFlow -= incr;
            if(depthOfFlow <= .5){
                theta = calcTheta(radius, depthOfFlow);
                area = calcArea(radius, theta);
                wettedPerimeter = calcWettedPerimeter(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }else{
                theta = calcTheta(radius, 1 - depthOfFlow);
                area = calcArea1(radius, theta);
                wettedPerimeter = calcWettedPerimeter1(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }
        }else if (flow  < desiredFlow - 1e-6){
            depthOfFlow += incr;
            if(depthOfFlow <= .5){
                theta = calcTheta(radius, depthOfFlow);
                area = calcArea(radius, theta);
                wettedPerimeter = calcWettedPerimeter(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }else{
                if(depthOfFlow > .9){
                    cout << endl
                         << "Depth of Flow Greater than .9." << endl
                         << "Calculation terminated early." << endl;
                    depthOfFlow = .9;
                    theta = calcTheta(radius, 1 - depthOfFlow);
                    area = calcArea1(radius, theta);
                    wettedPerimeter = calcWettedPerimeter1(radius, theta);
                    hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                    flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                    velocity = calcVelocity(flow, area);
                    break;
                }
                theta = calcTheta(radius, 1 - depthOfFlow);
                area = calcArea1(radius, theta);
                wettedPerimeter = calcWettedPerimeter1(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }
        }else{
            break;
        }
    }
}

void changingRoughnessDepthCalculator(double &theta, double &area, double &wettedPerimeter, double &hydraulicRadius, double &flow, double &velocity, double &depthOfFlow, double &radius,  double &frictionCoefficient, double &slope, double &desiredFlow, double &fpRoughness){
    double incr = .5;
    theta = calcTheta(radius, depthOfFlow);
    area = calcArea(radius, theta);
    wettedPerimeter = calcWettedPerimeter(radius, theta);
    hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
    frictionCoefficient = calcFrictionCoefficient(fpRoughness, depthOfFlow, radius);
    frictionCoefficient = fpRoughness;
    flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
    velocity = calcVelocity(flow, area);
    while(incr != 0){
        incr /= 2;
        if(flow > desiredFlow + 1e-6){
            depthOfFlow -= incr;
            if(depthOfFlow <= .5){
                theta = calcTheta(radius, depthOfFlow);
                area = calcArea(radius, theta);
                wettedPerimeter = calcWettedPerimeter(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                frictionCoefficient = calcFrictionCoefficient(fpRoughness, depthOfFlow, radius);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }else{
                theta = calcTheta(radius, 1 - depthOfFlow);
                area = calcArea1(radius, theta);
                wettedPerimeter = calcWettedPerimeter1(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                frictionCoefficient = calcFrictionCoefficient(fpRoughness, depthOfFlow, radius);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }
        }else if (flow  < desiredFlow - 1e-6){
            depthOfFlow += incr;
            if(depthOfFlow <= .5){
                theta = calcTheta(radius, depthOfFlow);
                area = calcArea(radius, theta);
                wettedPerimeter = calcWettedPerimeter(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                frictionCoefficient = calcFrictionCoefficient(fpRoughness, depthOfFlow, radius);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }else{
                if(depthOfFlow > .9){
                    // the depth of flow function changes to decreasing past .98, and values over .9 are not needed by user
                    cout << endl
                         << "Depth of Flow Greater than .9." << endl
                         << "Calculation terminated early." << endl;
                    depthOfFlow = .9;
                    theta = calcTheta(radius, 1 - depthOfFlow);
                    area = calcArea1(radius, theta);
                    wettedPerimeter = calcWettedPerimeter1(radius, theta);
                    hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                    frictionCoefficient = calcFrictionCoefficient(fpRoughness, depthOfFlow, radius);
                    flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                    velocity = calcVelocity(flow, area);
                    break;
                }
                theta = calcTheta(radius, 1 - depthOfFlow);
                area = calcArea1(radius, theta);
                wettedPerimeter = calcWettedPerimeter1(radius, theta);
                hydraulicRadius = calcHydraulicRadius(area, wettedPerimeter);
                frictionCoefficient = calcFrictionCoefficient(fpRoughness, depthOfFlow, radius);
                flow = calcFlow(area, hydraulicRadius, slope, frictionCoefficient);
                velocity = calcVelocity(flow, area);
            }
        }else{
            break;
        }
    }
}

double calcFlow(double area, double hydraulicRadius, double slope, double frictionCoefficient){
    return (area * pow(hydraulicRadius, 2.0/3) * sqrt(slope)) / frictionCoefficient;
}

double calcArea(double radius, double theta){
    return pow(radius, 2) * (theta - sin(theta))/2;
}

double calcArea1(double radius, double theta){
    return 3.14159265359 * pow(radius, 2) - pow(radius, 2) * (theta - sin(theta)) / 2;
}

double calcHydraulicRadius(double area, double wettedPerimeter){
    return area/wettedPerimeter;
}

double calcWettedPerimeter(double radius, double theta){
    return radius * theta;
}

double calcWettedPerimeter1(double radius, double theta){
    return 2 * 3.14159265359 * radius  - radius * theta;
}

double calcTheta(double radius, double depthOfFlow){
    return 2 * acos((radius - depthOfFlow * radius * 2) / radius);
}

double calcVelocity(double flow, double area){
    return flow / area;
}

double calcFrictionCoefficient(double fpRoughness, double depthOfFlow, double radius){
    if(depthOfFlow <= .03){
        return fpRoughness * (1 + depthOfFlow / radius / 2 / .3);
    }else if(depthOfFlow <= .1){
        return fpRoughness * (1.1 + (depthOfFlow / radius / 2 - .3)) * 12 / 7;
    }else if(depthOfFlow <= .2){
        return fpRoughness * (1.22 + (depthOfFlow / radius / 2 - .1)) * .6;
    }else if(depthOfFlow <= .3){
        return fpRoughness * 1.29;
    }else if(depthOfFlow <= .5){
        return fpRoughness * (1.29 - (depthOfFlow / radius / 2 - .3) * .2);
    }else{
        return fpRoughness * (1.25 - (depthOfFlow / radius / 2 - .5) * .5);
    }
}

void output1(double theta, double area, double wettedPerimeter, double hydraulicRadius, double flow, double velocity, double depthOfFlow, double radius){
    cout << "--------------------------------------------" << endl << fixed
         << "Calculated Values" << endl
         << "--------------------------------------------" << endl
         << left << setw(18) << "Theta: " << right << setprecision(4) << setw(9) << theta << " radians" << endl
         << left << setw(18) << "Area: " << right << setprecision(4) << setw(9) << area << " m^2" << endl
         << left << setw(18) << "Wetted Perimeter: " << right << setprecision(4) << setw(9) << wettedPerimeter << " m" << endl
         << left << setw(18) << "Hydraulic Radius: " << right << setprecision(4) << setw(9) << hydraulicRadius << " m" << endl
         << left << setw(18) << "Flow: " << right << setprecision(4) << setw(9) << flow << " m^3/s" << endl
         << left << setw(18) << "Velocity: " << right << setprecision(4) << setw(9) << velocity << " m/s" << endl
         << left << setw(18) << "Depth of Flow: " << right << setprecision(4) << setw(9) << depthOfFlow * 100 << " %" << endl
         << left << setw(18) << "Depth of Flow: " << right << setprecision(4) << setw(9) << depthOfFlow * radius * 2 << " m" << endl;
}

void output2(double theta, double area, double wettedPerimeter, double hydraulicRadius, double flow, double velocity, double depthOfFlow, double radius,  double frictionCoefficient){
    cout << "--------------------------------------------" << endl << fixed
         << "Calculated Values" << endl
         << "--------------------------------------------" << endl
         << left << setw(18) << "Theta: " << right << setprecision(4) << setw(9) << theta << " radians" << endl
         << left << setw(18) << "Area: " << right << setprecision(4) << setw(9) << area << " m^2" << endl
         << left << setw(18) << "Wetted Perimeter: " << right << setprecision(4) << setw(9) << wettedPerimeter << " m" << endl
         << left << setw(18) << "Hydraulic Radius: " << right << setprecision(4) << setw(9) << hydraulicRadius << " m" << endl
         << left << setw(18) << "Flow: " << right << setprecision(4) << setw(9) << flow << " m^3/s" << endl
         << left << setw(18) << "Velocity: " << right << setprecision(4) << setw(9) << velocity << " m/s" << endl
         << left << setw(18) << "Depth of Flow: " << right << setprecision(4) << setw(9) << depthOfFlow * 100 << " %" << endl
         << left << setw(18) << "Depth of Flow: " << right << setprecision(4) << setw(9) << depthOfFlow * radius * 2 << " m" << endl
         << left << setw(18) << "Friction Coefficient " << right << setprecision(4) << setw(9) << frictionCoefficient << endl;
}

void input(double &diameter,  double &fpRoughness, double &slope, double &desiredFlow){
    system("cls");
    cout << "--------------------------------------------" << endl
         << "Pipe Measurements" << endl
         << "--------------------------------------------" << endl;

    cout << "Diameter (meters): ";
    cin >> diameter;

    cout << "Full pipe roughness: ";
    cin >> fpRoughness;

    cout << "Slope (y/x): ";
    cin >> slope;

    cout << "Desired Flow (m^3/s): ";
    cin >> desiredFlow;
}

char select(){
    char input;
    cout << "--------------------------------------------" << endl
         << "Please select an option from the menu below by typing in the option's number." << endl
         << "--------------------------------------------" << endl
         << "1: Calculate depth using full pipe roughness." << endl
         << "2: Calculate depth using roughness based on depth." << endl
         << "3: Close the program." << endl;
    cin >> input;

    return input;
}
