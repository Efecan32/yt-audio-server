import express from "express";
import { execFile } from "child_process";
import { promisify } from "util";
import yts from "yt-search";

const execFileAsync = promisify(execFile);
const app = express();

// Health check endpoint
app.get("/", (req, res) => {
    res.status(200).json({ 
        status: "Server is running ✅", 
        timestamp: new Date(),
        endpoints: {
            play: "/play?q=<query>",
            health: "/"
        }
    });
});

// Main audio streaming endpoint
app.get("/play", async (req, res) => {
    const q = req.query.q;

    if (!q) {
        console.log("❌ Missing query parameter");
        return res.status(400).json({ error: "Query parameter 'q' is required" });
    }

    console.log(`🔍 Searching for: ${q}`);

    try {
        // Search for video using yt-search
        console.log(`📡 Searching on YouTube...`);
        const results = await yts(q);
        
        if (!results || !results.videos || results.videos.length === 0) {
            console.log("❌ No videos found");
            return res.status(404).json({ error: "No videos found for query" });
        }

        const video = results.videos[0];
        const videoUrl = `https://www.youtube.com/watch?v=${video.videoId}`;
        
        console.log(`✅ Found: "${video.title}"`);
        console.log(`👤 ${video.author.name} | ⏱️ ${video.seconds}s`);

        // Set streaming headers
        res.setHeader("Content-Type", "audio/mpeg");
        res.setHeader("Transfer-Encoding", "chunked");

        console.log(`🎵 Streaming audio with yt-dlp...`);
        
        // Use yt-dlp to stream audio
        const ytdlp = execFile("yt-dlp", [
            "-f", "bestaudio/best",
            "-x", "--audio-format", "mp3",
            "--audio-quality", "192",
            "-o", "-",
            videoUrl
        ], { maxBuffer: 10 * 1024 * 1024 });

        // Handle errors
        ytdlp.on("error", (error) => {
            console.error(`❌ yt-dlp error: ${error.message}`);
            if (!res.headersSent) {
                res.status(500).json({ error: "Failed to stream audio" });
            } else {
                res.end();
            }
        });

        // Pipe output to response
        ytdlp.stdout.pipe(res);

        res.on("finish", () => {
            console.log(`✅ Stream completed`);
            ytdlp.stdout.destroy();
        });

        res.on("close", () => {
            ytdlp.kill();
        });

    } catch (error) {
        console.error(`❌ Server error: ${error.message}`);
        if (!res.headersSent) {
            res.status(500).json({ error: "Internal server error", message: error.message });
        } else {
            res.end();
        }
    }
});

// Error handling middleware
app.use((err, req, res, next) => {
    console.error(`❌ Unhandled error: ${err.message}`);
    if (!res.headersSent) {
        res.status(500).json({ error: "Internal server error" });
    }
});

// 404 handler
app.use((req, res) => {
    res.status(404).json({ error: "Endpoint not found", available: { play: "/play?q=<query>", health: "/" } });
});

const PORT = process.env.PORT || 3000;
const server = app.listen(PORT, () => {
    console.log(`\n✅ YouTube Audio Server Started!\n`);
    console.log(`📡 Server running on port ${PORT}`);
    console.log(`🎵 Stream endpoint: GET http://localhost:${PORT}/play?q=<query>`);
    console.log(`💡 Health check: GET http://localhost:${PORT}/\n`);
});

// Graceful shutdown
process.on("SIGTERM", () => {
    console.log("SIGTERM signal received: closing HTTP server");
    server.close(() => {
        console.log("HTTP server closed");
        process.exit(0);
    });
});
