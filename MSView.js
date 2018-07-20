// JavaScript for Enbedded Space Demo
// Jim Skon, Kenyon College, 2017
var width=1200;
var height=600;
var scale = 1;
var savedXML="";
var levels;
var canvas;
var ctx;
var stage;
var linesLayer;
var indexLayer;
var nodeLayers = [];
var lines = [];
var radius = 5;
var totalNodes = 0;
var nodes = [];
//var nodes = new Array(33);
var texts = [];
var colorsUsed = 0;
var colors = ["#e6194b","#3cb44b","#ffe119","#0082c8","#f58231","#911eb4","#46f0ff","#f032e6","#d2f53c","#fabebe",
	      "#008080","#e6beff","#aa6e28","#fffac8","#800000","#aaffc3","#808000","#ffd8b1","#000080","#808080",
	      "#FFFFFF","#000000","#00194b","#00b44b","#00e119","#ff82c8","#008231","#001eb4","#fff0f0","#0032e6",
	      "#00f53c","#00bebe","#ff8080","#00beff","#00fac8","#ff0000","#00ffc3","#ff8000","#00d8b1",
	      "#fff080","#00FFFF","#990000"];
var nColors=colors.length;
var scaling = false;

$(document).ready(function () {
    createGraphArea();
    
    $('#DisplayButton').click(function() {
	if ($('#DisplayButton').attr("state")=="off") {
	    $('#DisplayButton').attr("state", "on");
	    $('#DisplayButton').text("Show Points");
	    $('#PointsList').hide();
	    $('#GraphView').show();
	} else {
	    $('#DisplayButton').attr("state","off");
	    $('#DisplayButton').text("Show Graph");
	    $('#PointsList').show();
	    $('#GraphView').hide();
	}
    });
    
    $("#PointsList").hide();
    $('#DisplayButton').attr("state", "on");
    
    $("#gen-btn").click(genPoints);

    $("#clear").click(clearResults);

    $("#scaledown").click(scaleDown);
    $("#scaleup").click(scaleUp);
    
    $(document).ajaxStart(function () {
	$("#loading").show();
    }).ajaxStop(function () {
	$("#loading").hide();
    });

});

function scalingStart() {
    $("#loading").show();
}

function scalingDone() {
    $("#loading").hide();
}



function storeCoordinate(xVal, yVal, array) {
    array.push({x: xVal, y: yVal});
}

function clearResults() {
    layer.removeChildren();
    indexLayer.removeChildren();
    //nodeLayer.removeChildren();
    //node.removeChildren();
    linesLayer.removeChildren();
    layer.draw();
    linesLayer.draw();
    indexLayer.draw();
    if ($('#Indexes').attr("state")=="off") {
	//indexLayer.hide();
	console.log("hide indexes");
    }
    texts=[];
    nodes=[];
    lines=[];
    clearNodes();
}
function clearNodes() {
    for (i=0;i<=nColors;i++) {
	nodeLayers[i].removeChildren();
	nodeLayers[i].draw();
	nodes[i]=new Array(0);
	//nodeLayer[i]=;
    }
}
function drawNodes() {
    for (i=0;i<=nColors;i++) {
	nodeLayers[i].draw();
    }
}

function showNodes() {
    for (i=0;i<=nColors;i++) {
	nodeLayers[i].show();
    }
}

function hideNodes() {
    for (i=0;i<=nColors;i++) {
	nodeLayers[i].hide();
    }
}

function updateGraphInfo() {
    var out = " Nodes: " + totalNodes;;
    out += ", Colors: "+colorsUsed+"  ";
    // display the number of nodes of each high-lighted color 
    for(var i=0;i<nodes.length;i++) {
	if (nodes[i].length>0) {
	    var size=nodes[i][0].getRadius();
	    if (size>radius) {
		out += "<font color='"+colors[i]+"'>("+i+","+(nodes[i].length)+")</font> ";
	    }
	}
    }
    $('#graphInfo').html(out);
    
}

function pointIndexes() {
    if ($('#Indexes').attr("state")=="off") {
	$('#Indexes').attr("state", "on");
	$('#Indexes').text("Hide Color Indexes");
	indexLayer.show();

    } else {
	$('#Indexes').attr("state","off");	    
	$('#Indexes').text("Show Color Indexes");
	indexLayer.hide();
    }
}

function ShowNodes() {
    //console.log("Ball");
    if ($('#ShowNodes').attr("state")=="off") {
	$('#ShowNodes').attr("state", "on");
	$('#ShowNodes').text("Hide Nodes");
	showNodes();
    } else {
	$('#ShowNodes').attr("state","off");	    
	$('#ShowNodes').text("Show Nodes");
	hideNodes();
   }
}

function ShowLines() {
    if ($('#ShowLines').attr("state")=="off") {
	$('#ShowLines').attr("state", "on");
	$('#ShowLines').text("Hide Lines");
	linesLayer.show();
    } else {
	$('#ShowLines').attr("state","off");	    
	$('#ShowLines').text("Show Lines");
	linesLayer.hide();
   }
    linesLayer.draw();
}

function scaleUp() {
    scaling = true;
    scalingStart();
    width=width*2;
    height = height*2;
    scale = scale*2;
    createGraphArea();
    processResults(savedXML);
    scaling = false;
    scalingDone();
}

function scaleDown() {
    scaling = true;
    scalingStart();
    width=width/2;
    height = height/2;
    scale=scale/2;
    createGraphArea();
    processResults(savedXML);
    scaling = false;
    scalingDone();
}


function createGraphArea() {
    stage = new Kinetic.Stage({
	container: 'Graph',
	width: width,
	height: height,
    });
    //console.log("Size:"+width+":"+height);
    layer = new Kinetic.Layer();
    indexLayer = new Kinetic.Layer();
    linesLayer = new Kinetic.Layer();
    nodeLayer = new Kinetic.Layer();
    nodes = new Array(0);
    for (i=0;i<=nColors;i++) {
	nodeLayers[i] = new Kinetic.Layer();
	stage.add(nodeLayers[i]);
	nodes[i]=new Array(0);
    }
//    stage.add(layer);
}

function addLine(x1,y1,x2,y2,layer) {
    var line = new Kinetic.Line({
	points: [x1,y1,x2,y2],
	stroke: 'black',
	strokeWidth: 1,
	lineCap: 'round',
	lineJoin: 'round'
    });
    lines.push(line);
    layer.add(line);

}

function bigNodes(index,size) {
    for(var i=0;i<nodes[index].length;i++) {
	nodes[index][i].setRadius(size);
    }
    nodeLayers[index].batchDraw();
}

function addNode(xp,yp,color,myRadius,layer,sig) {
    var circle = new Kinetic.Circle({
	x: xp,
	y: yp,
	radius: myRadius,
	fill: color,
	stroke: color,
	strokeWidth: 1,
	draggable: false
    });
	//console.log(color+":"+layer+":"+sig);
    nodeLayers[layer].add(circle);
    circle.index=layer;
    circle.on('click', function() {
	var size = this.getRadius();
	if (size > radius) {
	    bigNodes(this.index,radius);
	} else {
	    bigNodes(this.index,radius*2);
	}
	updateGraphInfo();
    });
    circle.on('mouseover',function() {
	 $('#sigOut').html("<"+sig+">");
    });
    circle.on('mouseout',function() {
	 $('#sigOut').html("");
    });
    return circle;
}

function addText(text,xp,yp,layer) {
    var outText = new Kinetic.Text({
	x: xp,
	y: yp,
	text: text,
	fontSize: 10,
	fontFamily: 'Arial',
	fill: 'black'
    });
    texts.push(outText);
    layer.add(outText);
}

function randomColor() {
    return ('#'+Math.floor(Math.random()*16777215).toString(16));
}

function callProcessResults(resultData) {
    savedData=resultData;
    processResults(resultData);
}
function processResults(resultData) {
    //console.log(resultData);
    var output = "";
    totalNodes=0;
    colorsUsed=0;
    nodes = new Array(0);
    clearResults();
    var count = 0;
    levels = $('#levels').val();
    var highlight = [];
    //radius = 7-levels;
    //var radius = 5;
    var pointColor = "Black";
    var s=0;
    var e=0;
    e=resultData.indexOf("\n",s);
    while (s < resultData.length) {
	line = resultData.substring(s,e);
	console.log("Line:"+line);
	s=e+1;
	e=resultData.indexOf("\n",s);
	
	var data = line.split(":");
	var x = parseInt(data[0])+10;
	var y = parseInt(data[1])+10;
	var color = parseInt(data[2]);
	var sig = data[4];
	//console.log(sig);
	x=x*scale;
	y=y*scale;
	totalNodes++;
	var node = addNode(x,y,colors[color],radius,color,sig);
	colorsUsed = Math.max(colorsUsed,color);
	nodes[color].push(node);
//	console.log(color+":"+nodes[color].length);
	addText(color,x+10,y-10,indexLayer);

	output+="["+count+"] x:"+x+" y:"+y;
	var links = data[3].split(",");
	for (i=0; i<links.size(); i+=2) {
	    addLine(x,y,(parseInt(links[i])+10)*scale,(parseInt(links[i+1])+10)*scale,linesLayer);
	    output+=" "+links[i]*scale+ "," + links[i+1]*scale;
	}

	output+="<br />";
	count++;
    }
    stage.add(layer);
    stage.add(linesLayer);
    stage.add(indexLayer);
    drawNodes();
    updateGraphInfo();
    $('#points').html(output+"<br />Total:"+count);
    */
}

function genPoints(){
    $('#points').empty();
    savedXML="";
    //if ($('#levels').val()>6) {
	//$('#levels').val(6);
    //}
    $.ajax({
	url: '/cgi-bin/skon_genGraphSig3.cgi?levels='+$('#levels').val(),
	dataType: 'text',
	success: callProcessResults,
	error: function(){alert("Error: Something went wrong");}
    });
}


