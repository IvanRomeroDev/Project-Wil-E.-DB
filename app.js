const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');

const PORT = process.env.PORT || 3000;

const app = express();

app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.post('/', (req, res) => {
    const user_data = req.body;
    console.log(user_data);
    res.sendFile(path.join(__dirname, 'public/index.html'));
});

app.listen(PORT);