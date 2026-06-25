#!/usr/bin/env python3
"""Generate structured project deep-dive pages (interview prep templates)."""

from pathlib import Path

ROOT = Path(__file__).resolve().parent

PROJECTS = [
    {
        "slug": "rag-pipeline",
        "title": "RAG Pipeline (LlamaIndex)",
        "status": "live",
        "status_label": "Live on GitHub",
        "category": "AI / ML",
        "intro": "AI-powered mortgage document assistant: upload a PDF and query it. Hybrid retrieval (vector + BM25) with cross-encoder reranking and Google Gemini for grounded answers.",
        "github": "https://github.com/MeganMulcahy/RAG-Pipeline-using-LlamaIndex",
        "technologies": [
            "Python", "LlamaIndex", "LangChain", "Google Gemini", "ChromaDB",
            "BM25", "Cross-encoder reranking", "PyMuPDF", "sentence-transformers", "RAGAs",
        ],
    },
    {
        "slug": "resume-scraper",
        "title": "Resume Scraper",
        "status": "progress",
        "status_label": "In progress",
        "category": "AI / ML",
        "intro": "Extracts structured resume data from PDFs and web sources for downstream parsing, scoring, and autofill workflows.",
        "github": "https://github.com/MeganMulcahy",
        "technologies": ["Python", "PyMuPDF", "PaddleOCR", "Tesseract", "FastAPI", "PostgreSQL"],
    },
    {
        "slug": "resume-autofill-ats",
        "title": "Resume Autofill & ATS Scoring",
        "status": "progress",
        "status_label": "In progress",
        "category": "AI / ML",
        "intro": "Autofills job application forms from a master resume profile and scores resume–job-description fit ATS-style with explainable keyword and semantic gaps.",
        "github": "https://github.com/MeganMulcahy",
        "technologies": [
            "Python", "OpenAI", "Google Gemini", "sentence-transformers",
            "Hugging Face", "FastAPI", "PostgreSQL", "LangChain",
        ],
    },
    {
        "slug": "instagram-scraper",
        "title": "Instagram Scraper",
        "status": "progress",
        "status_label": "In progress",
        "category": "AI / ML",
        "intro": "Collects public Instagram metadata and media for analysis pipelines — feeds the reel understanding and content intelligence stack.",
        "github": "https://github.com/MeganMulcahy",
        "technologies": ["Python", "HTTP clients", "Docker", "PostgreSQL", "Google Cloud"],
    },
    {
        "slug": "instagram-reel-llm",
        "title": "Instagram Reel LLM Analyzer",
        "status": "progress",
        "status_label": "In progress",
        "category": "AI / ML",
        "intro": "Multimodal pipeline over Instagram Reels — transcribes audio, summarizes content, and answers questions about trends and themes with LLMs.",
        "github": "https://github.com/MeganMulcahy",
        "technologies": [
            "Python", "Whisper", "OpenAI", "Google Gemini", "Ollama",
            "Hugging Face", "LangChain", "FFmpeg",
        ],
    },
    {
        "slug": "ml-projects",
        "title": "Machine Learning Projects",
        "status": "progress",
        "status_label": "Ongoing",
        "category": "AI / ML",
        "intro": "Collection of applied ML experiments — classification, embeddings, evaluation, and model comparison work supporting AI engineering interviews.",
        "github": "https://github.com/MeganMulcahy",
        "technologies": [
            "Python", "Hugging Face", "scikit-learn", "sentence-transformers",
            "Ollama", "Docker", "Google Cloud",
        ],
    },
    {
        "slug": "power-bi-license",
        "title": "Power BI License Dashboard",
        "status": "archive",
        "status_label": "Enterprise · Mercedes-Benz",
        "category": "Enterprise",
        "intro": "Dashboard integrating Excel, SharePoint, SQL, and cloud data to monitor license counts, allocation, and utilization with advanced DAX measures.",
        "github": None,
        "technologies": ["Power BI", "DAX", "SQL", "SharePoint", "Excel", "Google Cloud"],
    },
    {
        "slug": "power-bi-compliance",
        "title": "Power BI Compliance Dashboard",
        "status": "archive",
        "status_label": "Enterprise · Mercedes-Benz",
        "category": "Enterprise",
        "intro": "Compliance-focused reporting on license risks, expiration timelines, and audit readiness with drill-throughs and slicers for leadership.",
        "github": None,
        "technologies": ["Power BI", "DAX", "SQL", "SharePoint"],
    },
    {
        "slug": "power-apps-sla",
        "title": "Power Apps SLA/PO Workflow",
        "status": "archive",
        "status_label": "Enterprise · Mercedes-Benz",
        "category": "Enterprise",
        "intro": "Digitized SLA and PO approvals with Power Automate and SharePoint — routing, notifications, and approval logs.",
        "github": None,
        "technologies": ["Power Apps", "Power Automate", "SharePoint", "Microsoft 365"],
    },
    {
        "slug": "power-apps-audit",
        "title": "Power Apps Audit & Task Tracker",
        "status": "archive",
        "status_label": "Enterprise · Mercedes-Benz",
        "category": "Enterprise",
        "intro": "Centralized audit tasks and recurring compliance activities with reminders and history across departments.",
        "github": None,
        "technologies": ["Power Apps", "Power Automate", "SharePoint"],
    },
    {
        "slug": "cs-phd-portal",
        "title": "CS PhD Student Data Portal",
        "status": "archive",
        "status_label": "Academic · UA",
        "category": "Academic",
        "intro": "Web application for the CS department to collect PhD student data each semester. Stack: React, Node.js, PostgreSQL, REST API.",
        "github": None,
        "technologies": ["React", "Node.js", "PostgreSQL", "REST API"],
    },
    {
        "slug": "chocan",
        "title": "ChocAn Data Center",
        "status": "archive",
        "status_label": "Academic · UA",
        "category": "Academic",
        "intro": "Team-based Java system for a fictional healthcare provider — members, providers, services, and billing with UML and data persistence.",
        "github": None,
        "technologies": ["Java", "UML", "OOP", "File I/O", "JUnit"],
    },
]

BADGE_CLASS = {
    "live": "project-doc__badge--live",
    "progress": "project-doc__badge--progress",
    "archive": "project-doc__badge--archive",
}

TEMPLATE = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <title>{title} · Deep Dive · Megan Mulcahy</title>
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <link rel="preconnect" href="https://fonts.googleapis.com" />
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
  <link href="https://fonts.googleapis.com/css2?family=Cormorant+Garamond:wght@500;600&family=Inter:wght@400;500;600&display=swap" rel="stylesheet" />
  <link rel="stylesheet" href="../../style.css" />
  <link rel="stylesheet" href="../project-doc.css" />
</head>
<body class="project-doc-page">
  <nav class="project-doc-nav">
    <div class="project-doc-nav__inner">
      <a class="project-doc-nav__back" href="../../index.html#projects">← Back to portfolio</a>
      <div class="project-doc-nav__links">
        {github_link}
        <a href="../../index.html">Home</a>
      </div>
    </div>
  </nav>

  <article class="project-doc">
    <header class="project-doc__header">
      <p class="project-doc__eyebrow">{category} · Project deep dive</p>
      <h1 class="project-doc__title">{title}</h1>
      <div class="project-doc__meta">
        <span class="project-doc__badge {badge_class}">{status_label}</span>
      </div>
      <p class="project-doc__intro">{intro}</p>
    </header>

    <section class="project-doc__section" id="summary">
      <h2>Executive summary</h2>
      <p class="doc-placeholder">Write a 3–4 sentence pitch: what this project does, who it is for, and the main technical outcome. Use this as your 30-second interview opener.</p>
    </section>

    <section class="project-doc__section" id="problem">
      <h2>Problem &amp; motivation</h2>
      <p class="doc-placeholder">What pain point does this solve? Why did you build it? What would happen without this system?</p>
    </section>

    <section class="project-doc__section" id="architecture">
      <h2>System architecture</h2>
      <p class="doc-placeholder">Describe components (ingest, storage, retrieval, inference, UI/API). Add a diagram if helpful. Explain how data flows end-to-end.</p>
      <h3>Pipeline / workflow steps</h3>
      <ol>
        <li class="doc-placeholder" style="list-style: decimal;">Step 1 — (e.g., ingest, parse, chunk, embed…)</li>
        <li class="doc-placeholder" style="list-style: decimal;">Step 2 — …</li>
        <li class="doc-placeholder" style="list-style: decimal;">Step 3 — …</li>
      </ol>
    </section>

    <section class="project-doc__section" id="tech-deep-dive">
      <h2>Technology deep dive</h2>
      <p>For each tool below, fill in <strong>what it is</strong>, <strong>why you chose it</strong>, and <strong>how you use it in this project</strong>. This is your interview cheat sheet.</p>
      <div class="tech-deep-dive">
        {tech_blocks}
      </div>
    </section>

    <section class="project-doc__section" id="decisions">
      <h2>Key design decisions</h2>
      <p class="doc-placeholder">Document tradeoffs you made (e.g., hybrid retrieval vs. pure vector, cloud LLM vs. local, batch vs. streaming). Explain the &ldquo;deeper why&rdquo; behind each choice.</p>
    </section>

    <section class="project-doc__section" id="challenges">
      <h2>Challenges &amp; how I solved them</h2>
      <p class="doc-placeholder">What broke? What was harder than expected? How did you debug, measure, or iterate?</p>
    </section>

    <section class="project-doc__section" id="evaluation">
      <h2>Evaluation &amp; metrics</h2>
      <p class="doc-placeholder">How do you know it works? (accuracy, latency, RAGAs scores, ATS match rate, scrape success rate, etc.)</p>
    </section>

    <section class="project-doc__section" id="future">
      <h2>Future improvements</h2>
      <p class="doc-placeholder">What would you build next with more time? What would you refactor?</p>
    </section>

    <aside class="doc-callout">
      <h3>Interview talking points</h3>
      <p class="doc-placeholder" style="background: transparent; border: none; padding: 0; color: rgba(255,255,255,0.85); font-style: normal;">List 3–5 bullet points you want recruiters to remember after you explain this project.</p>
    </aside>
  </article>
</body>
</html>
"""

TECH_ITEM = """
        <div class="tech-deep-dive__item">
          <h4>{tech}</h4>
          <dl>
            <dt>What is it?</dt>
            <dd class="doc-placeholder" style="margin-bottom:0.5rem;border:none;box-shadow:none;padding:0.65rem 0.75rem;">Define {tech} in plain language.</dd>
            <dt>Why this project?</dt>
            <dd class="doc-placeholder" style="margin-bottom:0.5rem;border:none;box-shadow:none;padding:0.65rem 0.75rem;">Why {tech} over alternatives?</dd>
            <dt>How I use it here</dt>
            <dd class="doc-placeholder" style="border:none;box-shadow:none;padding:0.65rem 0.75rem;">Specific modules, APIs, configs, and data flow involving {tech}.</dd>
          </dl>
        </div>"""


def tech_blocks(technologies):
    return "".join(TECH_ITEM.format(tech=t) for t in technologies)


def github_link(url):
    if url and "github.com" in url and url.rstrip("/") != "https://github.com/MeganMulcahy":
        return f'<a href="{url}" target="_blank" rel="noopener noreferrer">GitHub repo</a>'
    return '<a href="https://github.com/MeganMulcahy" target="_blank" rel="noopener noreferrer">GitHub profile</a>'


def main():
    for project in PROJECTS:
        out_dir = ROOT / project["slug"]
        out_dir.mkdir(parents=True, exist_ok=True)
        html = TEMPLATE.format(
            title=project["title"],
            category=project["category"],
            status_label=project["status_label"],
            badge_class=BADGE_CLASS[project["status"]],
            intro=project["intro"],
            github_link=github_link(project.get("github")),
            tech_blocks=tech_blocks(project["technologies"]),
        )
        (out_dir / "index.html").write_text(html, encoding="utf-8")
        print(f"Wrote {out_dir / 'index.html'}")


if __name__ == "__main__":
    main()
