const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const fs = require('fs');
const subProcess = require('child_process');

/// compila el programa cuando se inicia el servidor
subProcess.execSync("g++ processor.cpp -o processor", (err, stdout, stderr) => {
    if (err) {
      console.error(err)
      process.exit(1)
    } else {
      console.log(`The stdout Buffer from shell: ${stdout.toString()}`)
      console.log(`The stderr Buffer from shell: ${stderr.toString()}`)
    }
});

const PORT = process.env.PORT || 3000;

const app = express();

app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.get('/respuesta', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/respuesta.html'));
})

app.post('/respuesta', (req, res) => {
    const user_data = req.body;
    
    /// creo el archivo con los datos
    fs.writeFileSync("data.txt", "", (err) => {
        if(err) throw err;
    });

    for(const prop in user_data){
            if(prop == "time") continue;
            fs.appendFileSync("data.txt", prop + ' ' + user_data[prop] + "\n", (err) => {
                if(err) throw err;
        });
    }
    
    /// ejecuto el programa en cpp desde la terminal
    subProcess.execFileSync("./processor", (err, stdout, stderr) => {
        if (err) {
          console.error(err)
          process.exit(1)
        } else {
          
          console.log(`The stdout Buffer from shell: ${stdout.toString()}`)
          console.log(`The stderr Buffer from shell: ${stderr.toString()}`)
        }
        
    });

    res.sendFile(path.join(__dirname, 'public/respuesta.html'));
});

app.listen(PORT);