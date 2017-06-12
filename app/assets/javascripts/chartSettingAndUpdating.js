$(document).ready(function () {
    drawWeekDayCompletionsVsGradesClassification(100);
    drawWeekendCompletionsVsGradesClassification(100);
    drawDayCompletionsVsGradesClassification(100);
    drawNightCompletionsVsGradesClassification(100);
    drawGradeAvgsChart();
    drawStudentsAmountPerClassChart();
    drawCompletedExcersisesAndGradesClustering(100);
    drawGradesSuccesrateChart(100);
    drawAttemptsVsFailures();

    createChartPercentageChoosers();
    createChartOutlierRemovers();
    createRegressionRadioBtns();

    handleSpoilerCollapseClicks();

    handleChartPercentageChoosersClicks();
    handleOutlierRemoverClicks();
    handleLinearRegressionLineChoosers();
    handleLogarithmicRegressionLineChoosers()
});

//note that these are refs to existing functions
var updateFunctionsPerChart =
    {
        "completedexcersisesandgradesclustering": drawCompletedExcersisesAndGradesClustering,
        "weekdaycompletionsvsgradesclassification": drawWeekDayCompletionsVsGradesClassification,
        "gradesuccessrate": drawGradesSuccesrateChart,
        "weekendcompletionsvsgradesclassification": drawWeekendCompletionsVsGradesClassification,
        "daycompletionsvsgradesclassification": drawDayCompletionsVsGradesClassification,
        "nightcompletionsvsgradesclassification": drawNightCompletionsVsGradesClassification
    };


/*
CHOOSER CREATIONS
*/

function createChartPercentageChoosers() {
    var percentageChooserHtml =
        '<p>Grade Percentage: </p>' +
        '<input type="number" class="percentageChooserValue btn btn-default" max="100"></input><br />' +
        '<input type="button" class="percentageChooserBtn btn btn-default" value="Confirm" style="margin-top:10px"></input>';

    $('.updatableChart').next('.panel1').contents('.panel-collapse').contents('.panel-body').append(percentageChooserHtml);
}

function createChartOutlierRemovers() {
    var outlierRemoverHMTL =
        '<br /> <input type="button" style="margin-top:10px" class="outlierRemover btn btn-success" value="Remove outliers"></input>' +
        '<input type="button" style="margin-top:10px" class="outlierAdder btn btn-danger" value="Add outliers"></input>';

    $('.Clustering').next('.panel1').contents('.panel-collapse').contents('.panel-body').append(outlierRemoverHMTL);
}

function createRegressionRadioBtns() {
    var RegressionRadioBtnsHtml =
        '<p style="margin-top: 20px;">Add/Remove Linear Regression</p>' +
        '<input type="checkbox" class="LinearRegressionButton checkbox"></input>' +
        '<p style="margin-top: 20px;">Add/Remove Logarithmic Regression</p>' +
        '<input type="checkbox" class="LogarithmicRegressionButton checkbox"></input>';

    $('.RegressionChart').next('.panel1').contents('.panel-collapse').contents('.panel-body').append(RegressionRadioBtnsHtml);
}

function createStatisticalMeasurements(chart, chartId) {
    if ($(chartId).hasClass('CorrelationChart') == true) {
        drawCorrelation(chart, chartId);
    }
}



function handleSpoilerCollapseClicks() {
    $(".spoiler-trigger").click(function () {
        $(this).parent().next().collapse('toggle');
    });
}

function handleOutlierRemoverClicks() {
    $(".outlierRemover").click(function () {
        var associatedChartId = $(this).parent().parent().parent().prev('.Clustering').attr('id');
        removeOutliersUsingDBSCAN(associatedChartId);
    });

    $(".outlierAdder").click(function () {
        var associatedChartId = $(this).parent().parent().parent().prev('.Clustering').attr('id');
        var associatedChartPercentageValue = $(this).parent().parent().parent().prev('.Clustering').next('.panel1').next('.percentageChooserValue').val();

        drawOriginalGraph(associatedChartId, associatedChartPercentageValue);
    });
}

function handleChartPercentageChoosersClicks() {
    $('.percentageChooserBtn').click(function () {
        var newPercentageValue = $(this).prev().prev().val();
        var associatedChartId = $(this).parent().parent().parent().prev('.updatableChart').attr('id');
        console.log("newPercentageValue = " + newPercentageValue);

        updateFunctionsPerChart[associatedChartId](newPercentageValue);

        //unchecking/removing regressions
        var linearRegressionButtonForChart = $(this).parent().parent().parent().prev('.RegressionChart')
            .next('.panel1').contents('.panel-collapse').contents('.panel-body').find('.LinearRegressionButton').prop('checked', false);

        var logRegressionButtonForChart = $(this).parent().parent().parent().prev('.RegressionChart')
            .next('.panel1').contents('.panel-collapse').contents('.panel-body').find('.LogarithmicRegressionButton').prop('checked', false);
    });
}

function handleLinearRegressionLineChoosers() {
    $('.LinearRegressionButton').click(function () {
        createLinearRegressionIfButtonChecked(this);
    });
}

function handleLogarithmicRegressionLineChoosers() {
    $('.LogarithmicRegressionButton').click(function () {
        createLogarithmicRegressionIfButtonChecked(this);
    });
}




/*
OUTLIER REMOVAL, PEARSON, SPEARMAN, REGRESSIONAND PERCENTAGE RUNTIME
*/

function removeOutliersUsingDBSCAN(associatedChartId) {
    var myChart = $('#' + associatedChartId).highcharts();
    var allValuesArray = getAllSeriesData(myChart);

    $.get("http://localhost:3000/home/filteroutliers/[" + allValuesArray + "]", function (data) {
        //remove all series, replace by new one
        var seriesLength = myChart.series.length;
        for (var i = seriesLength - 1; i > -1; i--) {
            myChart.series[i].remove();
        }

        var seriesWithoutOutliers = parseJSONClusterData(data);
        for (var j = 0; j < seriesWithoutOutliers.length; j++)
            myChart.addSeries(seriesWithoutOutliers[j]);

        myChart.redraw();
    });
}

function drawOriginalGraph(associatedChartId, associatedChartPercentageValue) {
    if (typeof associatedChartPercentageValue != 'undefined')
        updateFunctionsPerChart[associatedChartId](associatedChartPercentageValue);
    else
        updateFunctionsPerChart[associatedChartId](100);
}



function drawCorrelation(chart, chartId) {
    var allValuesArray = getAllSeriesData(chart);

    $.get("http://localhost:3000/home/correlation/[" + allValuesArray + "]", function (data) {
        //Very literal JSON get
        var pearson = 'Pearson Correlation: ' + data[0]["pearsonCoefficient"];
        var spearman = 'Spearman Correlation: ' + data[1]["spearmanCoefficient"];

        $(chartId).next().next('.panel2').contents('.panel-collapse').contents('.panel-body').find('#pearson').text(pearson);
        $(chartId).next().next('.panel2').contents('.panel-collapse').contents('.panel-body').find('#spearman').text(spearman);
    });
}


function createLinearRegressionIfButtonChecked(button) {
    //DUPED
    var associatedChartId = $(button).parent().parent().parent().prev('.RegressionChart').attr('id');
    var myChart = $('#' + associatedChartId).highcharts();

    if ($(button).is(':checked')) {
        var allValuesArray = getAllSeriesData(myChart);

        $.get("http://localhost:3000/home/linearregression/[" + allValuesArray + "]", function (data) {
            drawLinearRegression(myChart, data);
        });
    }
    else {
        removeLinearRegression(myChart);
    }
}

function createLogarithmicRegressionIfButtonChecked(button) {
    //DUPED
    var associatedChartId = $(button).parent().parent().parent().prev('.RegressionChart').attr('id');
    var myChart = $('#' + associatedChartId).highcharts();

    if ($(button).is(':checked')) {
        var allValuesArray = getAllSeriesData(myChart);

        $.get("http://localhost:3000/home/logregression/[" + allValuesArray + "]", function (data) {
            drawLogarithmicRegression(myChart, data);
        });
    }
    else {
        removeLogarithmicRegression(myChart);
    }
}

//DUPED
function drawLinearRegression(chart, data) {
    var seriesObj =
        {
            type: 'line',
            id: 'LinearRegression',
            name: 'Linear Regression',
            color: 'rgba(255, 0, 0, 0.70)',
            lineWidth: 1.75,
            allowPointSelect: false,
            dataLabels: {
                enabled: false
            },
            markers: {
                enabled: false
            },
            data: []
        };

    seriesObj.data = parseJSONXYData(data);
    chart.addSeries(seriesObj);
}

//DUPED
function drawLogarithmicRegression(chart, data) {
    var seriesObj =
        {
            type: 'line',
            id: 'LogarithmicRegression',
            name: 'Logarithmic Regression',
            color: 'rgba(0, 0, 255, 0.70)',
            lineWidth: 1.75,
            allowPointSelect: false,
            dataLabels: {
                enabled: false
            },
            markers: {
                enabled: false
            },
            data: []
        };

    seriesObj.data = parseJSONXYData(data);
    chart.addSeries(seriesObj);
}

function removeLogarithmicRegression(chart) {
    chart.get('LogarithmicRegression').remove();
}

function removeLinearRegression(chart) {
    chart.get('LinearRegression').remove();
}




function tuplesArrayComparator(a, b) {
    if (a[1] < b[1]) return -1;
    if (a[1] > b[1]) return 1;
    return 0;
}

function parseJSONXYData(data) {
    var dataArr = [];
    for (i = 0; i < data.length; i++) {
        var x = parseFloat(data[i].x);
        var y = parseFloat(data[i].y);
        dataArr.push([x, y]);
    }
    dataArr = dataArr.sort(tuplesArrayComparator);
    return dataArr;
}

function parseJSONClusterData(data) {
    var series = new Array();
    for (i = 0; i < data.length; i++) {
        var currSeriesObj = { name: "Cluster " + (i + 1), data: [] };

        var currClusterDataArray = data[i]["data"];
        for (j = 0; j < currClusterDataArray.length; j++) {
            //add data to current series
            var x = parseFloat(currClusterDataArray[j].x);
            var y = parseFloat(currClusterDataArray[j].y);

            currSeriesObj.data.push([x, y]);
        }
        series.push(currSeriesObj);
    }
    return series;
}

function getAllSeriesData(chart) {
    allSeriesPoints = Array();
    for (i = 0; i < chart.series.length; i++) {
        //we dont use added regression data, only the original points
        if (chart.series[i].name.indexOf("Regression") == -1) {
            var currentChartData = chart.series[i]['data'];
            for (j = 0; j < currentChartData.length; j++) {
                var x = parseFloat(currentChartData[j].x);
                var y = parseFloat(currentChartData[j].y);
                allSeriesPoints.push([x, y]);
            }
        }
    }
    [].concat.apply([], allSeriesPoints);
    return allSeriesPoints;
}



/*
CHART DRAWING
*/

function getChartOptions(optionsObject, chartId) {
    var options = {
        chart: {
            type: optionsObject.type,
            events: {
                load: function () {
                    createStatisticalMeasurements(this, chartId);
                },
                redraw: function () {
                    createStatisticalMeasurements(this, chartId);
                }
            }
        },
        title: {
            text: optionsObject.title
        },
        xAxis: {
            categories: optionsObject.xLabels,
            title: {
                text: optionsObject.xText
            }
        },
        yAxis: {
            min: 0,
            max: 100,
            categories: optionsObject.yLabels,
            title: {
                text: optionsObject.yText
            }
        },
        plotOptions: {
        },
        legend: {
            layout: 'vertical',
            align: 'right',
            verticalAlign: 'middle'
        },
        series: []
    };
    return options;
}

function drawGradeAvgsChart() {

    $(function () {
        $.getJSON('http://localhost:3000/home/gradeavgs', function (data) {
            console.log("drawGradeAvgsChart");

            var chartId = '#gradeaverages';

            var optionObj = {
                title: "Average grade per class",
                type: 'column',
                xText: 'Class no.',
                yText: 'Average grade',
                xLabels: []
            };

            var firstSeriesObj = { name: "Average grade", data: [] }
            for (i = 0; i < data.length; i++) {
                var parsedValFloat = parseFloat(data[i].y);

                optionObj.xLabels.push("Class no. " + data[i].x);
                firstSeriesObj.data.push(parsedValFloat);
            }

            var chartOptions = getChartOptions(optionObj, chartId);
            chartOptions.series.push(firstSeriesObj);

            $(chartId).highcharts(chartOptions);
        });
    });
}

function drawStudentsAmountPerClassChart() {

    $(function () {
        $.getJSON('http://localhost:3000/home/amountofstudentsperclass', function (data) {
            console.log("drawStudentsAmountPerClassChart");

            var chartId = '#amountofstudentsperclass';

            var optionObj = {
                title: "Amount of students per class",
                type: 'column',
                xText: 'Class no.',
                yText: 'Amount of students',
                xLabels: []
            };

            var firstSeriesObj = { name: "Amount of students", data: [] }
            for (i = 0; i < data.length; i++) {
                var parsedValFloat = parseFloat(data[i].y);

                optionObj.xLabels.push("Class no. " + data[i].x);
                firstSeriesObj.data.push(parsedValFloat);
            }

            var chartOptions = getChartOptions(optionObj, chartId);
            chartOptions.series.push(firstSeriesObj);

            $(chartId).highcharts(chartOptions);
        });
    });
}

function drawCompletedExcersisesAndGradesClustering(studentsGradePercentage) {

    $(function () {
        $.get('http://localhost:3000/home/kmeans/' + studentsGradePercentage, function (data) {
            console.log("drawCompletedExcersisesAndGradesClustering");

            var chartId = '#completedexcersisesandgradesclustering';

            var options = {
                title: "Amount of completed excersises per student vs. grades per student clustering",
                type: 'scatter',
                xText: 'Amount of completed excersises',
                yText: 'Grade',
            };
            var optionObj = getChartOptions(options, chartId);

            optionObj.series = parseJSONClusterData(data);

            //done to ensure outlier removal or classifications with filters do not look skewed
            //ptionObj.xAxis.max = 500;

            $(chartId).highcharts(optionObj);
        }).error(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        }).fail(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        });
    });
}

function drawWeekDayCompletionsVsGradesClassification(studentsGradePercentage) {
    $(function () {
        $.get('http://localhost:3000/home/weekdaycompletionsvsgradesclassification/' + studentsGradePercentage, function (data) {
            console.log("drawWeekendCompletionsVsGradesClassification");

            var chartId = '#weekdaycompletionsvsgradesclassification';

            var options = {
                title: "Week day only excersise completions vs. grades classification (K Nearest Neighbours)",
                type: 'scatter',
                xText: 'Amount of excersises completed during the week (Mon-Fri)',
                yText: 'Grade',
            };
            var optionObj = getChartOptions(options, chartId);
            optionObj.series = parseJSONClusterData(data);

            //done to ensure outlier removal or classifications with filters do not look skewed
            //optionObj.xAxis.max = 500;

            $(chartId).highcharts(optionObj);
        }).error(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        }).fail(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        });
    });
}

function drawWeekendCompletionsVsGradesClassification(studentsGradePercentage) {
    $(function () {
        $.get('http://localhost:3000/home/weekendcompletionsvsgradesclassification/' + studentsGradePercentage, function (data) {
            console.log("drawWeekendCompletionsVsGradesClassification");

            var chartId = '#weekendcompletionsvsgradesclassification';

            var options = {
                title: "Weekend only excersise completions vs. grades classification (K Nearest Neighbours)",
                type: 'scatter',
                xText: 'Amount of excersises completed during the weekend (Sat/Sun)',
                yText: 'Grade',
            };
            var optionObj = getChartOptions(options, chartId);
            optionObj.series = parseJSONClusterData(data);

            //done to ensure outlier removal or classifications with filters do not look skewed
            //optionObj.xAxis.max = 500;

            $(chartId).highcharts(optionObj);
        }).error(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        }).fail(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        });
    });
}


function drawDayCompletionsVsGradesClassification(studentsGradePercentage) {

    $(function () {
        $.get('http://localhost:3000/home/daycompletionsvsgradesclassification/' + studentsGradePercentage, function (data) {
            console.log("drawDayCompletionsVsGradesClassification");

            var chartId = '#daycompletionsvsgradesclassification';

            var options = {
                title: "Daytime only excersise completions vs. grades classification (Naive Bayes and KNN combination)",
                type: 'scatter',
                xText: 'Amount of excersises completed during the night (6AM - 22PM)',
                yText: 'Grade',
            };
            var optionObj = getChartOptions(options, chartId);
            optionObj.series = parseJSONClusterData(data);

            //done to ensure outlier removal or classifications with filters do not look skewed
            //optionObj.xAxis.max = 500;

            $(chartId).highcharts(optionObj);
        }).error(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        }).fail(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);
        });;
    });
}

function drawNightCompletionsVsGradesClassification(studentsGradePercentage) {

    $(function () {
        $.get('http://localhost:3000/home/nightcompletionsvsgradesclassification/' + studentsGradePercentage, function (data) {
            var chartId = '#nightcompletionsvsgradesclassification';

            var options = {
                title: "Nighttime only excersise completions vs. grades classification (Naive Bayes and KNN combination)",
                type: 'scatter',
                xText: 'Amount of excersises completed during the night (22PM - 6AM)',
                yText: 'Grade',
            };
            var optionObj = getChartOptions(options, chartId);
            optionObj.series = parseJSONClusterData(data);

            //done to ensure outlier removal or classifications with filters do not look skewed
            //optionObj.xAxis.max = 500;

            $(chartId).highcharts(optionObj);
        }).error(function (jqXHR, textStatus, errorThrown) {
            console.log("ERROR THROWN: " + errorThrown);

            // Etc
        });
    });
}



function drawGradesSuccesrateChart(studentsGradePercentage) {
    $(function () {
        $.getJSON('http://localhost:3000/home/successrate/' + studentsGradePercentage, function (data) {
            var chartId = '#gradesuccessrate';

            var optionObj = {
                title: "Grades and amount of excersise successes per student",
                type: 'scatter',
                xText: 'Completed excersises amount',
                yText: 'Grade'
            };

            var chartOptions = getChartOptions(optionObj, chartId);
            chartOptions.series = [];

            var xSeriesObj = { name: "Grades and amount of excersise successes", data: [] };

            for (i = 0; i < data.length; i++) {
                var parsedKeyFloat = parseFloat(data[i]["data"].x);
                var parsedValFloat = parseFloat(data[i]["data"].y);

                xSeriesObj.data.push([parsedKeyFloat, parsedValFloat]);
            }
            chartOptions.series.push(xSeriesObj);

            $(chartId).highcharts(chartOptions);
        });
    });
}

function drawAttemptsVsFailures() {
    $(function () {
        $.getJSON('http://localhost:3000/home/attemptsvsfailures/', function (data) {
            var chartId = '#attemptsvsfailures';

            var optionObj = {
                title: "Attempts vs failures per student",
                type: 'scatter',
                xText: 'Total attempts',
                yText: 'failures'
            };

            var chartOptions = getChartOptions(optionObj, chartId);
            chartOptions.series = [];
            chartOptions.yAxis.max = 6000;

            var xSeriesObj = { name: "Attempts vs failures per student", data: [] };

            var allDataPoints = Array();
            for (i = 0; i < data.length; i++) {
                var parsedKeyFloat = parseFloat(data[i]["data"].x);
                var parsedValFloat = parseFloat(data[i]["data"].y);

                console.log(parsedKeyFloat + "," + parsedValFloat);

                allDataPoints.push([parsedKeyFloat, parsedValFloat]);
            }

            xSeriesObj.data = allDataPoints;
            chartOptions.series.push(xSeriesObj);

            $(chartId).highcharts(chartOptions);
        });
    });
}
