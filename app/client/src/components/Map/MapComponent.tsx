import React, { useEffect, useState } from 'react';
import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';
import type { Bin } from '../../types';

// Custom Scalable Vector Graphics (SVG) Marker
const createCustomIcon = (color: string) => {
    const svgIcon = `
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="36" height="36" fill="${color}">
      <path d="M12 0C7.58 0 4 3.58 4 8c0 5.25 7 13 8 13s8-7.75 8-13c0-4.42-3.58-8-8-8zm0 11c-1.66 0-3-1.34-3-3s1.34-3 3-3 3 1.34 3 3-1.34 3-3 3z"/>
    </svg>`;

    return L.divIcon({
        className: 'custom-marker',
        html: svgIcon,
        iconSize: [36, 36],
        iconAnchor: [18, 36], // Bottom center
        popupAnchor: [0, -38], // Above the icon
    });
};

const greenIcon = createCustomIcon('#10B981'); // Emerald Green
const yellowIcon = createCustomIcon('#FBBF24'); // Amber/Yellow
const redIcon = createCustomIcon('#EF4444');   // Red

const center: [number, number] = [46.7712, 23.6236]; // Cluj-Napoca

const MapComponent: React.FC = () => {
    const [bins, setBins] = useState<Bin[]>([]);

    useEffect(() => {
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
                })
                .catch(error => {
                    console.error('Error fetching bins:', error);
                });
        };

        fetchBins();
        // Set up polling every 30 seconds
        const interval = setInterval(fetchBins, 30000);
        return () => clearInterval(interval);
    }, []);

    return (
        <MapContainer
            center={center}
            zoom={13}
            style={{ height: '100%', width: '100%', borderRadius: '15px' }}
        >
            <TileLayer
                attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors &copy; <a href="https://carto.com/attributions">CARTO</a>'
                url="https://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}{r}.png"
            />
            {bins.map((bin) => {
                // Logic for marker color
                // Red: Last updated more than 1 hour ago
                // Yellow: Fill level > 80% (and recently updated)
                // Green: Otherwise
                let icon = greenIcon;
                let statusColor = '#047857'; // Green text
                let statusBg = '#ECFDF5';   // Green bg

                const lastUpdatedDate = new Date(bin.lastUpdated);
                const oneHourAgo = new Date(Date.now() - 60 * 60 * 1000);

                if (lastUpdatedDate < oneHourAgo) {
                    icon = redIcon;
                    statusColor = '#B91C1C';
                    statusBg = '#FEF2F2';
                } else if (bin.fillLevel > 80) {
                    icon = yellowIcon;
                    statusColor = '#B45309'; // Amber text
                    statusBg = '#FFFBEB';   // Amber bg
                }

                return (
                    <Marker key={bin.id} position={[bin.lat, bin.lng]} icon={icon}>
                        <Popup>
                            <div style={{ color: '#333', textAlign: 'center' }}>
                                <div style={{
                                    marginTop: '5px',
                                    padding: '4px 8px',
                                    borderRadius: '4px',
                                    background: statusBg,
                                    color: statusColor,
                                    fontWeight: 'bold'
                                }}>
                                    Fill Level: {bin.fillLevel}%
                                </div>
                            </div>
                        </Popup>
                    </Marker>
                );
            })}
        </MapContainer>
    );
};

export default MapComponent;
