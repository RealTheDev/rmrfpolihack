import React, { Suspense } from 'react';
import type { Bin } from '../../types';
// import MapComponent from '../../components/Map/MapComponent'; // Removed static import
import './LandingPage.css';

// Lazy load the MapComponent
const MapComponent = React.lazy(() => import('../../components/Map/MapComponent'));



const LandingPage: React.FC = () => {
    const [bins, setBins] = React.useState<Bin[]>([]);
    const [activeUnits, setActiveUnits] = React.useState<number>(0);

    React.useEffect(() => {
        window.scrollTo(0, 0);

        const fetchBins = () => {
            fetch('https://rmrfpolihack.onrender.com/api/bins')
                .then(response => {
                    if (!response.ok) {
                        throw new Error('Failed to fetch bins');
                    }
                    return response.json();
                })
                .then((data: any[]) => {
                    console.log('Fetched bins data:', data);
                    const mappedBins: Bin[] = data.map(item => ({
                        id: item.binId,
                        lat: item.latitude,
                        lng: item.longitude,
                        fillLevel: item.fillLevel,
                        lastUpdated: item.lastUpdated
                    }));
                    setBins(mappedBins);
                    updateActiveUnits(mappedBins);
                })
                .catch(error => {
                    console.error('Error fetching bins:', error);

                });
        };

        const updateActiveUnits = (currentBins: Bin[]) => {
            const oneHourAgo = new Date(Date.now() - 60 * 60 * 1000);
            const activeCount = currentBins.filter(bin => {
                const lastUpdatedDate = new Date(bin.lastUpdated);
                return lastUpdatedDate >= oneHourAgo;
            }).length;
            setActiveUnits(activeCount);
        };

        fetchBins();
        // Set up polling every 30 seconds
        const interval = setInterval(fetchBins, 30000);
        return () => clearInterval(interval);

    }, []);

    return (
        <div className="landing-page">
            <header className="navbar">
                <div className="logo">
                    SmartCan
                </div>
                <div className="nav-actions">
                    <button className="download-btn" onClick={() => document.getElementById('map')?.scrollIntoView({ behavior: 'smooth' })}>
                        View Map
                    </button>
                </div>
            </header>

            <section className="hero">
                <div className="hero-text">
                    <h1>SMART <br /> WASTE <br /> MANAGEMENT</h1>
                    <p className="subtitle">
                        Revolutionizing city cleanliness with IoT-enabled intelligent trash cans. Real-time monitoring for a greener future.
                    </p>
                    <button className="cta-btn" onClick={() => document.getElementById('map')?.scrollIntoView({ behavior: 'smooth' })}>
                        Explore Live Map
                    </button>
                </div>

                <div className="hero-image-container">
                    <img src="/hero-trash-can.png" alt="Smart Trash Can" className="hero-image" />
                </div>
            </section>

            {/* Wave Divider */}
            <div className="wave-container">
                <svg viewBox="0 0 1440 320" xmlns="http://www.w3.org/2000/svg">
                    <path fill="#F8F9FD" fillOpacity="1" d="M0,96L48,112C96,128,192,160,288,186.7C384,213,480,235,576,213.3C672,192,768,128,864,128C960,128,1056,192,1152,208C1248,224,1344,192,1392,176L1440,160L1440,320L1392,320C1344,320,1248,320,1152,320C1056,320,960,320,864,320C768,320,672,320,576,320C480,320,384,320,288,320C192,320,96,320,48,320L0,320Z"></path>
                </svg>
            </div>

            {/* Impact Section */}
            <section className="impact-section">
                <div className="section-header">
                    <h2>Why SmartCan?</h2>
                    <p>Driving efficiency and sustainability for modern cities.</p>
                </div>
                <div className="impact-grid">
                    <div className="impact-card">
                        <div className="icon">🌱</div>
                        <h3>Eco-Friendly</h3>
                        <p>Reducing carbon footprint by optimizing collection routes and reducing overflow.</p>
                    </div>
                    <div className="impact-card">
                        <div className="icon">⚡</div>
                        <h3>Operational Efficiency</h3>
                        <p>Save costs and time with real-time fill-level data and predictive analytics.</p>
                    </div>
                    <div className="impact-card">
                        <div className="icon">📊</div>
                        <h3>Data Driven</h3>
                        <p>Comprehensive insights into waste generation patterns to inform city planning.</p>
                    </div>
                </div>
            </section>

            <section id="map" className="map-section">
                <div className="map-content">
                    <div className="map-header">
                        <h2>Live City Monitoring</h2>
                        <p>Real-time fleet tracking across Cluj-Napoca.</p>
                    </div>

                    <div className="map-card-wrapper">
                        <div className="map-card">
                            <Suspense fallback={
                                <div style={{
                                    height: '100%',
                                    width: '100%',
                                    display: 'flex',
                                    justifyContent: 'center',
                                    alignItems: 'center',
                                    background: '#f3f4f6',
                                    borderRadius: '15px',
                                    color: '#6b7280',
                                    fontWeight: 500
                                }}>
                                    Loading Map...
                                </div>
                            }>
                                <MapComponent bins={bins} />
                            </Suspense>
                        </div>
                        <div className="map-stats-sidebar">
                            <div className="stat-box">
                                <span className="stat-number">{activeUnits}</span>
                                <span className="stat-desc">Active Units</span>
                            </div>
                            <div className="stat-box">
                                <span className="stat-number">85%</span>
                                <span className="stat-desc">Collection Eff.</span>
                            </div>
                            <div className="stat-box">
                                <span className="stat-number">24/7</span>
                                <span className="stat-desc">Monitoring</span>
                            </div>
                        </div>
                    </div>
                </div>
            </section>

            <footer className="footer">
                <div className="footer-content">
                    <div className="footer-brand">
                        <span className="logo-text">SmartCan</span>
                        <p>Making cities smarter, one can at a time.</p>
                    </div>
                    <div className="footer-links">
                        <a href="#">Privacy Policy</a>
                        <a href="#">Terms of Service</a>
                        <a href="#">Contact Support</a>
                    </div>
                    <div className="footer-socials">
                        <span>© 2025 SmartCan Inc.</span>
                    </div>
                </div>
            </footer>
        </div>
    );
};

export default LandingPage;
