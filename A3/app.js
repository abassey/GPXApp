'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

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

