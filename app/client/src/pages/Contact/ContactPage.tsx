import React from 'react';
import { useNavigate } from 'react-router-dom';
import './ContactPage.css';

const ContactPage: React.FC = () => {
    const navigate = useNavigate();

    React.useEffect(() => {
        window.scrollTo(0, 0);
    }, []);

    return (
        <div className="contact-page">
            <header className="navbar">
                <div className="logo" onClick={() => navigate('/')} style={{ cursor: 'pointer' }}>
                    SmartCan
                </div>
                <div className="nav-actions">
                    <button className="download-btn" onClick={() => navigate('/')}>
                        Home
                    </button>
                </div>
            </header>

            <section className="contact-section">
                <h1>Contact Us</h1>
                <div className="contact-list">
                    <div className="contact-item">
                        <h2>Vladi: <a href="tel:+40753574538">+40 753 574 538</a></h2>
                    </div>
                    <div className="contact-item">
                        <h2>Gheorg: <a href="tel:+40755361924">+40 755 361 924</a></h2>

                    </div>
                    <div className="contact-item">
                        <h2>Bogdan: <a href="tel:+40771056822">+40 771 056 822</a></h2>
                    </div>
                    <div className="contact-item">
                        <h2>Andrei: <a href="tel:+40774420931">+40 774 420 931</a></h2>
                    </div>
                    <div className="contact-item">
                        <h2>Octavian: <a href="tel:+40757934947">+40 757 934 947</a></h2>
                    </div>
                </div>
            </section>
        </div>
    );
};

export default ContactPage;
