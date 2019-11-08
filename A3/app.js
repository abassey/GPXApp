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
	'rename_route': ['string', ['string', 'string']],
});

app.get('/gpx', function(req, res){
	
	var r = [];
	
	let files = fs.readdirSync('./uploads');
	
	for(var i = 0; i < files.length; i++)
	{
		
		let c = lib.gpx_struct_to_html(files[i]);
	
		let jsonObj = JSON.parse(c);
		jsonObj["filename"] = files[i];
	
		r[i] = JSON.stringify(jsonObj);
	}
	

	res.send(r);
});

app.get('/rte/:filename', function(req , res){


    let file = req.params.filename;

    console.log(file);

    let c = lib.rte_struct_to_html(file);
    console.log(c);


   res.send(c);
});

app.get('/trk/:filename', function(req , res){


    let file = req.params.filename;

    console.log(file);

    let c = lib.trk_struct_to_html(file);
    console.log(c);


   res.send(c);
});

app.post('/gpxcreate/:filename/:gpxJSON', function(req , res){


    let file = req.params.filename;

    console.log(file);

    let c = lib.html_to_gpx_struct(file, req.params.gpxJSON);
    console.log(c);


   res.send(c);
});

app.post('/rename/:filename/:rteJSON', function(req , res){


    let file = req.params.filename;

    console.log(file);

    let c = lib.rename_route(file, req.params.rteJSON);
    console.log(c);


   res.send(c);
});


