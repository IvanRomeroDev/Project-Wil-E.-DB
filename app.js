const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const fs = require('fs');

const PORT = process.env.PORT || 3000;

const app = express();

app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.post('/', (req, res) => {
    const user_data = req.body;

    /**
     * User region
     * budget
     * disk space
     * monthly petitions
     * server approach
     */
    
    /// creo el archivo con los datos
    fs.writeFile("data.txt", "", (err) => {
        if(err) throw err;
    });

    for(const prop in user_data){
        fs.appendFile("data.txt", user_data[prop] + "\n", (err) => {
            if(err) throw err;
        });
    }
    
    /// ejecuto el programa en cpp desde la terminal
    
    /// leo la respuesta
    const response = require("./response.json");
    for(const prop in response){
        console.log(prop + ": " + response[prop]);
    }

    /// envio la respuesta al cliente
    res.send(JSON.stringify(response));
});

app.listen(PORT);