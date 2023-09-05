const http = require('http');
const fs = require('fs');
const path = require('path');

const server = http.createServer((req, res) => {
    // Set the content type to HTML
    res.setHeader('Content-Type', 'text/html');

    // Read and serve the HTML file
    fs.readFile(path.join(__dirname, 'index.html'), 'utf8', (err, data) => {
        if (err) {
            console.error(err);
            res.statusCode = 500;
            res.end('Internal Server Error');
        } else {
            res.statusCode = 200;
            res.end(data);
        }
    });
});

const PORT = process.env.PORT || 3000; // You can use any available port

server.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
