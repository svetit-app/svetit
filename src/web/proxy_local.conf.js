const protractor = require('./protractor.conf.js')

const proxy_remote_server_address = 'http://localhost:8080';
const proxy_remote_ws_server_address = 'ws://localhost:8080';

let PROXY_CONFIG = [
    {
        context: [
            '/en',
            '/fr',
            '/ru'
        ],
        target: protractor.config.baseUrl,
        secure: false,
        pathRewrite: {
            '^/en': '',
            '^/fr': '',
            '^/ru': ''
        }
    },
    {
        context: [
            '/api',
            '/get_csrf',
            '/export',
            '/media'
        ],
        target: proxy_remote_server_address,
        secure: false,
    }
]

if (proxy_remote_server_address.indexOf('localhost') != -1)
{
    PROXY_CONFIG.unshift(
    {
        context: '/api/v2',
        pathRewrite: {
            '^/api/v2': ''
        },
        target: 'http://localhost:8123',
        secure: false,
        changeOrigin: true,
        headers: { "X-Real-IP":"127.0.0.1" }
    })

    PROXY_CONFIG.push(
    {
        context: '/ws/',
        // pathRewrite: {
        //     '^/ws*': '',
        // },
        target: proxy_remote_ws_server_address,
        secure: false,
        ws: true,
        changeOrigin: true,
        logLevel: "debug"
    })
}
else
{
    PROXY_CONFIG.push(
    {
        context: '/ws/',
        pathRewrite: {
            '^/ws*': '/wss',
        },
        target: proxy_remote_server_address,
        secure: false,
        ws: true,
        changeOrigin: true
    })
}

module.exports = PROXY_CONFIG
