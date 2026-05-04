# 🎵 YouTube Audio Server

A Node.js Express server that streams audio from YouTube videos. Search for any video and get the audio stream directly!

## ✨ Features

- 🎵 **Audio Streaming**: Stream MP3 audio from YouTube videos
- 🔍 **Video Search**: Built-in YouTube video search functionality
- ⚡ **Production Ready**: Configured for Render.com deployment
- 🏥 **Health Check**: `/` endpoint to verify server status
- 📊 **Detailed Logging**: Clear console output for debugging

## 📋 Requirements

- **Node.js** >= 18.0.0
- **FFmpeg** (for audio conversion)
- **yt-dlp** (for YouTube streaming)

## 🚀 Local Development

### Installation

```bash
npm install
```

### Development Server

```bash
npm start
```

The server will start on `http://localhost:3000`

### API Endpoints

#### Health Check
```bash
GET http://localhost:3000/
```

Response:
```json
{
  "status": "Server is running ✅",
  "timestamp": "2026-05-04T21:30:00.000Z",
  "endpoints": {
    "play": "/play?q=<query>",
    "health": "/"
  }
}
```

#### Stream Audio
```bash
GET http://localhost:3000/play?q=song+name
```

Query Parameters:
- `q` (required): Search query for the YouTube video

Example:
```bash
curl "http://localhost:3000/play?q=never+gonna+give+you+up" --output song.mp3
```

Response:
- **Content-Type**: `audio/mpeg`
- **Body**: MP3 audio stream

## 🌐 Deployment on Render.com

### Steps

1. **Connect GitHub Repository**
   - Go to [Render Dashboard](https://dashboard.render.com)
   - Click "New +"
   - Select "Web Service"
   - Connect your GitHub repository

2. **Render Configuration**
   - Render will automatically detect `render.yaml`
   - Environment: Node.js
   - Build Command: Automatically set from `render.yaml`
   - Start Command: Automatically set from `render.yaml`

3. **Deploy**
   - Push to GitHub
   - Render automatically deploys on every push to `main`

### Environment Variables

The following are automatically set:
- `NODE_ENV`: `production`
- `PORT`: Set by Render (auto-assigned)

## 📝 How It Works

1. **Search Phase**: Uses `yt-search` to find the top YouTube video matching the query
2. **Stream Phase**: Uses `yt-dlp` to extract and stream the audio
3. **Pipe**: Audio streams directly to the client as MP3

## 🔧 Dependencies

- **express**: Web server framework
- **yt-search**: YouTube video search
- **ffmpeg**: Audio codec support
- **yt-dlp**: YouTube content extraction

## 🛠 Troubleshooting

### "yt-dlp command not found"
- Local: Install via `apt-get install yt-dlp` or package manager
- Render: Already handled in `render.yaml`

### "No videos found"
- Check query parameter spelling
- Try a different search term
- Ensure internet connection

### Port Already in Use
- The server uses environment variable `PORT`
- Default: 3000
- To change: `PORT=8000 npm start`

## 📄 License

ISC

---

**🚀 Ready for Render deployment!**

For issues or questions, create a GitHub issue or PR!
