
const fetch = require('node-fetch'); // Might need to check if node-fetch is available or use native fetch if node 18+

// Using native fetch if available (Node 18+) or standard http
const http = require('http');

http.get('http://localhost:5000/api/bins', (res) => {
    let data = '';
    res.on('data', (chunk) => {
        data += chunk;
    });
    res.on('end', () => {
        console.log(data);
    });
}).on('error', (err) => {
    console.log('Error: ' + err.message);
});
