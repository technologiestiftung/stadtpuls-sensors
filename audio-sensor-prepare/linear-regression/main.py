# https://www.geeksforgeeks.org/linear-regression-python-implementation/
# https://circuitdigest.com/microcontroller-projects/arduino-sound-level-measurement
import numpy as np
import matplotlib.pyplot as plt


def estimate_coef(x, y):
    # number of observations/points
    n = np.size(x)

    # mean of x and y vector
    m_x = np.mean(x)
    m_y = np.mean(y)

    # calculating cross-deviation and deviation about x
    SS_xy = np.sum(y*x) - n*m_y*m_x
    SS_xx = np.sum(x*x) - n*m_x*m_x

    # calculating regression coefficients
    b_1 = SS_xy / SS_xx
    b_0 = m_y - b_1*m_x

    return (b_0, b_1)


def plot_regression_line(x, y, b):
    # plotting the actual points as scatter plot
    plt.scatter(x, y, color="m",
                marker="o", s=30)

    # predicted response vector
    y_pred = b[0] + b[1]*x

    # plotting the regression line
    plt.plot(x, y_pred, color="g")
    plt.axvline(75, color='k', linestyle='dashed', linewidth=1)
    plt.axhline(1380, color='k', linestyle='dashed', linewidth=1)
    # putting labels
    plt.xlabel('dB')
    plt.ylabel('ADC')
    plt.savefig('plot.png')
    # function to show plot
    plt.show()


def main():
    csv = np.genfromtxt('adc-samples.csv', delimiter=',')
    # print(csv)

    # observations / data
    adcs = []
    dbs = []
    for i in range(1, len(csv)):
        # for arr in csv:
        # print(arr[0])
        adcs.append(csv[i][0])
        dbs.append(csv[i][1])

    print(adcs, dbs)
    #x = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
    #y = np.array([1, 3, 2, 5, 7, 8, 8, 9, 10, 12])
    np_dbs = np.array(dbs)
    np_adcs = np.array(adcs)

    # estimating coefficients
    b = estimate_coef(np_dbs, np_adcs)
    print("Estimated coefficients:\nb_0 = {}  \
          \nb_1 = {}".format(b[0], b[1]))

    # plotting regression line
    plot_regression_line(np_dbs, np_adcs, b)


if __name__ == "__main__":
    main()
