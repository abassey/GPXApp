'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');


const mysql = require('mysql2/promise');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//Sample endpoints

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

let lib = ffi.Library('./libgpxparse', {
	'gpx_struct_to_html': ['string', ['string']],
	'rte_struct_to_html': ['string', ['string']],
	'trk_struct_to_html': ['string', ['string']],
	'points_struct_to_html': ['string', ['string']],
	'html_to_gpx_struct': ['string', ['string', 'string']],
	'rename_route': ['void', ['string', 'string', 'int']],
	'rename_track': ['void', ['string', 'string', 'int']],
	'all_rtes_between': ['string', ['string', 'float', 'float', 'float', 'float', 'float']],
	'all_trks_between': ['string', ['string', 'float', 'float', 'float', 'float', 'float']],
	'add_route_from_form': ['void', ['string', 'string', 'string', 'int']],

});

app.get('/gpx', function(req, res){
	
	var r = [];
	
	let files = fs.readdirSync('./uploads');
	
	for(var i = 0; i < files.length; i++)
	{
		
		let c = lib.gpx_struct_to_html(files[i]);
		
		if(c == "Invalid file") continue;
	
		let jsonObj = JSON.parse(c);
		jsonObj["filename"] = files[i];
	
		r[i] = JSON.stringify(jsonObj);
	}
	

	res.send(r);
});

app.get('/rte/:filename', function(req , res){


    let file = req.params.filename;

    //console.log(file);

    let c = lib.rte_struct_to_html(file);
    //console.log(c);


   res.send(c);
});

app.get('/trk/:filename', function(req , res){


    let file = req.params.filename;

    //console.log(file);

    let c = lib.trk_struct_to_html(file);
    //console.log(c);


   res.send(c);
});

app.get('/gpxcreate', function(req , res){


    let file = req.query.filename;

    console.log(file);

   let c = lib.html_to_gpx_struct(file, req.query.gpxJSON);
    console.log(c);


   res.send(c);
});

app.get('/rename-rte', function(req , res){


	let file = req.query.filename;
	console.log(file);
    lib.rename_route(file, req.query.newname, req.query.index);
});

app.get('/rename-trk', function(req , res){


	let file = req.query.filename;
	console.log(file);
    lib.rename_track(file, req.query.newname, req.query.index);
});

app.get('/inbetween-rte', function(req , res){

	
	console.log(req.query);

	var r = [];
	
	let files = fs.readdirSync('./uploads');
	
	for(var i = 0; i < files.length; i++)
	{
		
		let c = lib.all_rtes_between(files[i], req.query.startLat, req.query.startLong, req.query.endLat, req.query.endLong, req.query.tol);
	
		let jsonObj = JSON.parse(c);
		jsonObj["filename"] = files[i];
	
		r[i] = JSON.stringify(jsonObj);
		console.log(r[i]);
	}
	

	res.send(r);
});

app.get('/inbetween-trk', function(req , res){

	
	console.log(req.query);

	var r = [];
	
	let files = fs.readdirSync('./uploads');
	
	for(var i = 0; i < files.length; i++)
	{
		
		let c = lib.all_trks_between(files[i], req.query.startLat, req.query.startLong, req.query.endLat, req.query.endLong, req.query.tol);
	
		let jsonObj = JSON.parse(c);
		jsonObj["filename"] = files[i];
	
		r[i] = JSON.stringify(jsonObj);
		console.log(r[i]);
		
		
	}
	
	res.send(r);
});

app.get('/add-route', function(req , res){


    let file = req.query.filename;

    console.log(file);

	lib.add_route_from_form(file, req.query.routeName, req.query.waypoints, req.query.length);
    
	res.send('Success');
});

//*********************A4 Functionality********************************

let cred = {};
let connection; 
let query_file = `CREATE TABLE IF NOT EXISTS FILE (
	gpx_id INT NOT NULL AUTO_INCREMENT, 
	file_name VARCHAR(60) NOT NULL, 
	ver DECIMAL(2,1) NOT NULL, 
	creator VARCHAR(256) NOT NULL, PRIMARY KEY (gpx_id))`;
	
let query_route = `CREATE TABLE IF NOT EXISTS ROUTE (
	route_id INT NOT NULL AUTO_INCREMENT, 
	route_name VARCHAR(256), 
	route_len FLOAT(15,7) NOT NULL, 
	gpx_id INT NOT NULL,
	PRIMARY KEY (route_id),
	FOREIGN KEY (gpx_id) REFERENCES FILE (gpx_id))`;
	
let query_point = `CREATE TABLE IF NOT EXISTS POINT (
	point_id INT NOT NULL AUTO_INCREMENT, 
	point_index INT NOT NULL, 
	latitude DECIMAL(15,7) NOT NULL, 
	longitude DECIMAL(15,7) NOT NULL, 
	point_name VARCHAR(256), 
	route_id INT NOT NULL,
	PRIMARY KEY (point_id), 
	FOREIGN KEY (route_id) REFERENCES ROUTE(route_id))`;
	
//working, but do a logout button if possible	
app.get('/dbms', async function(req, res){
	
	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
		//console.log(req.query.dbname);
		var status;
	try
	{  	
		connection = await mysql.createConnection(cred);
			
		await connection.execute(query_file);
		await connection.execute(query_route);
		await connection.execute(query_point);

		status = "Success";
		
	} catch(err) {  
		
			console.log("Query error: " + err);
			status = "Fail";
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}
	
	console.log(status);
	res.send(status);
});

//working
app.get('/delete-all', async function(req, res){

	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
	try
	{  	
		connection = await mysql.createConnection(cred);
		
		console.log('Success');
			res.status(200).send('Connected!');
		
		await connection.execute("DELETE FROM POINT");
		await connection.execute("DELETE FROM ROUTE");
		await connection.execute("DELETE FROM FILE");

		
	} catch(e) {  
		
			console.log("Query error: " + e);
			res.status(400).send('Could not connect. ' + e);
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}
	
});

//working
app.get('/display-rtes', async function(req, res){

	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
		var r = [];
		var i = 0;
		console.log(req.query.choice);
		let routeSort = 'SELECT * from `ROUTE` ORDER BY ' + '`'+ req.query.choice +'`;';
		console.log(routeSort);
		
	try
	{  	
		connection = await mysql.createConnection(cred);
		
		console.log('Success');
		
		const [rows, fields] = await connection.execute(routeSort);
		
	
		for (let row of rows){
		r[i] = {"route_name": row.route_name, "route_len": row.route_len};
		console.log(r[i]);
		i++;
		}
		
	} catch(e) {  
		
			console.log("Query error: " + e);
			
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}
	
	res.send(r);
});

//working
app.get('/display-rtes-file', async function(req, res){

	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
		var r = [];
		var i = 0;
		let routeSort = 'SELECT * from `ROUTE` WHERE gpx_id=(SELECT gpx_id FROM FILE WHERE file_name=\'' + req.query.filename +'\''+') ORDER BY ' + '`'+ req.query.choice +'`;';
		console.log(routeSort);
		
	try
	{  	
		connection = await mysql.createConnection(cred);
		
		console.log('Success');
		
		const [rows, fields] = await connection.execute(routeSort);
		
	
		for (let row of rows){
		r[i] = {"route_name": row.route_name, "route_len": row.route_len};
		console.log(r[i]);
		i++;
		}

		
	} catch(e) {  
		
			console.log("Query error: " + e);
			
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}
	
	res.send(r);
});

//working
app.get('/display-points-rte', async function(req, res){

	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
		var r = [];
		var i = 0;
		let pointSort = 'SELECT * from `POINT` WHERE route_id=(SELECT route_id FROM `ROUTE` WHERE gpx_id=(SELECT gpx_id FROM FILE WHERE file_name=\'' + req.query.filename +'\')'+' AND route_name=\''+ req.query.rtechoice  + '\') ORDER BY `point_index`;';
		console.log(pointSort);
		
	try
	{  	
		connection = await mysql.createConnection(cred);
		
		//console.log('Success');
		
		const [rows, fields] = await connection.execute(pointSort);
		
	
		for (let row of rows){
		r[i] = {"point_index": row.point_index, "point_name": row.point_name, "latitude": row.latitude, "longitude": row.longitude};
		console.log(r[i]);
		i++;
		}

		
	} catch(e) {  
		
			console.log("Query error: " + e);
			
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}
	
	res.send(r);
});

//to test
app.get('/display-points-file', async function(req, res){

	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
		var r = [];
		var i = 0;
		let pointSort = 'SELECT * from `POINT` WHERE route_id=(SELECT route_id FROM `ROUTE` WHERE gpx_id=(SELECT gpx_id FROM FILE WHERE file_name=\'' + req.query.filename +'\')) ORDER BY `point_index`;';
		console.log(pointSort);
		
	try
	{  	
		connection = await mysql.createConnection(cred);
		
		//console.log('Success');
		
		const [rows, fields] = await connection.execute(pointSort);
		
	
		for (let row of rows){
		r[i] = {"point_index": row.point_index, "point_name": row.point_name, "latitude": row.latitude, "longitude": row.longitude};
		console.log(r[i]);
		i++;
		}

		
	} catch(e) {  
		
			console.log("Query error: " + e);
			
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}
	
	res.send(r);
});

//working for files and routes and points
app.get('/storefiles', async function(req, res){
	
	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
		console.log(req.query.dbname);
		var r = [];
		let files = fs.readdirSync('./uploads');
	try
	{  	
		connection = await mysql.createConnection(cred);
		
		res.status(200).send('Connected!');
		
		for(var i = 0; i < files.length; i++)
		{
		
			let c = lib.gpx_struct_to_html(files[i]);
			
			if(c == "Invalid file") continue;
		
			let jsonObj = JSON.parse(c);
			jsonObj["filename"] = files[i];
			
			//console.log(jsonObj);
			
			let fr = 'INSERT IGNORE INTO FILE (file_name, ver, creator) VALUES (\'' + jsonObj["filename"] + '\',\'' + jsonObj["version"] + '\',\'' + jsonObj["creator"] +'\');';
			//console.log(fr);
			await connection.execute(fr);
			
			let d = lib.rte_struct_to_html(files[i]);
			let rteObj = JSON.parse(d);
			console.log(rteObj);
			
			for(var j = 0; j < rteObj.length; j++)
			{
				//console.log(rteObj[j]);
				let gpxq = 'SELECT (SELECT gpx_id FROM FILE WHERE file_name=\''+files[i]+'\') AS gpx;';
				//console.log(gpxq);
				let [rows, fields] = await connection.execute(gpxq);
				//console.log(rows[0].gpx);
				let rr = 'INSERT IGNORE INTO ROUTE (route_name, route_len, gpx_id) VALUES (\'' + rteObj[j]["name"] + '\',\'' + rteObj[j]["len"] + '\', \'' + rows[0].gpx + '\');';
				await connection.execute(rr);
			}
			
			//error is somewhere here
			let e = lib.points_struct_to_html(files[i]);
			let pointObj = JSON.parse(e);
			console.log(pointObj[0]["point_info"][0]["point_index"]);
			
			for(var j = 0; j < pointObj.length; j++)
			{
				//console.log(rteObj[j]);
				let rteq = 'SELECT (SELECT route_id FROM ROUTE WHERE route_name=\''+pointObj[j]["route_name"]+'\') AS rte;';
				console.log(rteq);
				let [rows, fields] = await connection.execute(rteq);
				console.log(rows[0].rte);
				for(var k = 0; k < pointObj[j]["point_info"].length; k++)
				{
					let pr = 'INSERT IGNORE INTO POINT (point_index, longitude, latitude, point_name, route_id) VALUES (\'' + pointObj[j]["point_info"][k]["point_index"] + '\',\'' + pointObj[j]["point_info"][k]["lon"] + '\', \'' + pointObj[j]["point_info"][k]["lat"] + '\',\'' + pointObj[j]["point_info"][k]["name"] + '\',\'' + rows[0].rte + '\');';
					console.log(pr);
					await connection.execute(pr);
				}
				
			}
			
		}

		
	} catch(e) {  
		
			console.log("Query error: " + e);
			//alert(e);
			res.status(400).send('Could not connect. ' + e);
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}

});

//working
app.get('/db-status', async function(req, res){

	cred = {    
		host     : 'dursley.socs.uoguelph.ca',    
		user     : req.query.username,   
		password : req.query.password,   
		database : req.query.dbname  }
		
		var r = [];
		
	try
	{  	
		connection = await mysql.createConnection(cred);
		
		console.log('Success');
		
		let [rows1, fields1] = await connection.execute('SELECT (SELECT COUNT(*) FROM FILE) AS fc;');
		let [rows2, fields2] = await connection.execute('SELECT (SELECT COUNT(*) FROM ROUTE) AS rc;');
		let [rows3, fields3] = await connection.execute('SELECT (SELECT COUNT(*) FROM POINT) AS pc;');
		
		for (let row of rows1){
            console.log(row.fc);
            r[0] = row.fc;
        }
		for (let row of rows2){
            console.log(row.rc);
            r[1] = row.rc;
        }
		for (let row of rows3){
            console.log(row.pc);
            r[2] = row.pc;
        }

	} catch(e) {  
		
			console.log("Query error: " + e);
			
	}finally{
		     //Close the connection  
		     
			if (connection && connection.end) connection.end();  
	}
	
	res.send(r);
});
