
$(document).ready(function () {
    drawGradeAvgsChart();
    drawStudentsAmountPerClassChart();
    drawCompletedExcersisesAndGradesClustering(100);
    drawGradesSuccesrateChart(100);

    createChartPercentageChoosers();
    createRegressionRadioBtns();

    handleSpoilerCollapseClicks();

    handleChartPercentageChoosersClicks();
    handleLinearRegressionLineChoosers();
    handleLogarithmicRegressionLineChoosers()
});



/*
CHOOSER CREATIONS
*/

function createChartPercentageChoosers() {
    var percentageChooserHtml =
        '<p>Grade Percentage: </p>' +
        '<input type="number" class="percentageChooserValue" max="100"></input>' +
        '<input type="button" class="percentageChooserBtn" value="Confirm"></input>';

    $('.updatableChart').next('.panel1').contents('.panel-collapse').contents('.panel-body').append(percentageChooserHtml);
}

function createRegressionRadioBtns() {
    var RegressionRadioBtnsHtml =
        '<p style="margin-top: 20px;">Add/Remove Linear Regression</p>' +
        '<input type="checkbox" class="LinearRegressionButton"></input>' +
        '<p style="margin-top: 20px;">Add/Remove Logarithmic Regression</p>' +
        '<input type="checkbox" class="LogarithmicRegressionButton"></input>';

    $('.RegressionChart').next('.panel1').contents('.panel-collapse').contents('.panel-body').append(RegressionRadioBtnsHtml);
}

function handleSpoilerCollapseClicks() {
    $(".spoiler-trigger").click(function () {
        $(this).parent().next().collapse('toggle');
    });
}

function createStatisticalMeasurements(chart, chartId) {
    if ($(chartId).hasClass('CorrelationChart') == true) {
        drawCorrelation(chart, chartId);
    }
}




/*
PEARSON, SPEARMAN, REGRESSION AND PERCENTAGE RUNTIME
*/


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

function handleChartPercentageChoosersClicks() {
    //note that these are refs to existing functions
    var updateFunctionsPerChart =
        {
            "completedexcersisesandgradesclustering": drawCompletedExcersisesAndGradesClustering,
            "gradesuccessrate": drawGradesSuccesrateChart
        };

    $('.percentageChooserBtn').click(function () {
        var newPercentageValue = $(this).prev().val();
        var associatedChartId = $(this).parent().parent().parent().prev('.updatableChart').attr('id');

        updateFunctionsPerChart[associatedChartId](newPercentageValue);

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
    console.log("createLogarithmicRegressionIfButtonChecked");

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
                    //not very clean
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

            chartOptions.yAxis.max = 100;
            chartOptions.yAxis.min = 0;


            $(chartId).highcharts(chartOptions);
        });
    });
}

function drawStudentsAmountPerClassChart() {
    $(function () {
        $.getJSON('http://localhost:3000/home/amountofstudentsperclass', function (data) {
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

            chartOptions.yAxis.max = 100;
            chartOptions.yAxis.min = 0;


            $(chartId).highcharts(chartOptions);
        });
    });
}

function drawCompletedExcersisesAndGradesClustering(studentsGradePercentage) {
    $(function () {
        $.get('http://localhost:3000/home/kmeans/' + studentsGradePercentage, function (data) {
            var options = {
                title: "Amount of correct excersise answers per student vs. grades per student",
                type: 'scatter',
                xText: 'Amount of correct excersise answers',
                yText: 'Grade',
            };
            var optionObj = getChartOptions(options);
            optionObj.yAxis.max = 100;
            optionObj.yAxis.min = 0;


            for (i = 0; i < data.length; i++) {
                var currSeriesObj = { name: "Cluster " + (i + 1), data: [] };

                var currClusterDataArray = data[i]["data"];
                for (j = 0; j < currClusterDataArray.length; j++) {
                    //add data to current series
                    var x = parseFloat(currClusterDataArray[j].x);
                    var y = parseFloat(currClusterDataArray[j].y);

                    currSeriesObj.data.push([x, y]);
                }
                optionObj.series.push(currSeriesObj);
            }
            $('#completedexcersisesandgradesclustering').highcharts(optionObj);
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
            chartOptions.yAxis.max = 100;
            chartOptions.yAxis.min = 0;


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