export default {
  async fetch(request) {
    const url = new URL(request.url);
    const q = (url.searchParams.get("s") || "").toLowerCase();

    let radio = "Süper FM";
    let stream = "https://playerservices.streamtheworld.com/api/livestream-redirect/SUPER_FM.mp3";

    // 📻 RADYO LİSTESİ
    if (q.includes("kral")) {
      radio = "Kral Pop";
      stream = "https://dygedge.radyotvonline.net/kralpop/playlist.m3u8";
    }
    else if (q.includes("power")) {
      radio = "Power FM";
      stream = "https://listen.powerapp.com.tr/powerfm/mpeg/icecast.audio";
    }
    else if (q.includes("slow")) {
      radio = "Slow Türk";
      stream = "https://radyo.duhnet.tv/slowturk";
    }
    else if (q.includes("metro")) {
      radio = "Metro FM";
      stream = "https://17753.live.streamtheworld.com/METRO_FM.mp3";
    }
    else if (q.includes("fenomen")) {
      radio = "Fenomen FM";
      stream = "https://live.radyofenomen.com/fenomen/128/icecast.audio";
    }
    else if (q.includes("joy")) {
      radio = "Joy Türk";
      stream = "https://playerservices.streamtheworld.com/api/livestream-redirect/JOY_TURK.mp3";
    }
    else if (q.includes("super") || q.includes("süper")) {
      radio = "Süper FM";
      stream = "https://playerservices.streamtheworld.com/api/livestream-redirect/SUPER_FM.mp3";
    }
    else if (q.includes("trt")) {
      radio = "TRT FM";
      stream = "https://trtcanliradyolar-lh.akamaihd.net/i/TRT_FM_1@182840/master.m3u8";
    }

    return new Response(
      JSON.stringify({
        status: "ok",
        query: q,
        radio: radio,
        stream: stream
      }),
      {
        headers: { 
          "Content-Type": "application/json",
          "Access-Control-Allow-Origin": "*"
        }
      }
    );
  }
};
