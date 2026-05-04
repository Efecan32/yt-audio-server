import express from "express";
import ytdl from "ytdl-core";
import fetch from "node-fetch";

const app = express();

app.get("/play", async (req, res) => {
    const q = req.query.q;

    if (!q) {
        return res.status(400).send("Query parameter 'q' is required");
    }

    try {
        const r = await fetch(`https://ytsearch.vercel.app/api?q=${q}`);
        const data = await r.json();

        if (!data || data.length === 0) {
            return res.status(404).send("No videos found");
        }

        const videoUrl = data[0].url;

        res.setHeader("Content-Type", "audio/mpeg");
        ytdl(videoUrl, { filter: "audioonly" }).pipe(res);

    } catch (e) {
        console.error(e);
        res.status(500).send("Error processing request");
    }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
