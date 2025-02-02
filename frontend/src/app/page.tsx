import Link from "next/link";

export default function Home() {
  return (
    <div className="flex flex-col relative">

      <div className="h-screen flex flex-col gap-3 items-center justify-center text-center">
        <div className="flex flex-col items-center gap-2">
          <h1 className="text-3xl font-medium w-[600px]">Analyze Web Interactions to Understand Cognitive and Emotional Well-Being</h1>
          <p className="text-base w-[900px]">SynapSense is an advanced AI tool designed for mental health researchers to analyze digital interactions and behavioral patterns. By leveraging state-of-the-art NLP models, including BERT-based sentiment analysis, SynapSense detects emotional cues and cognitive trends in real-time web activity.</p>
        </div>
        <div className="flex gap-3">
          <Link href="/visualize" className="flex items-center px-2 py-1 rounded-md text-xl bg-black text-white hover:bg-black/80 transition-colors">Get Started</Link>
          <Link href="#demo" className="flex items-center px-2 py-1 rounded-md text-xl border border-black hover:bg-black/10 transition-colors">Try Demo</Link>
        </div>
      </div>

    </div>
  );
}
