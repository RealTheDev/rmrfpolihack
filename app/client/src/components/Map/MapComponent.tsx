import React from 'react';
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
const redIcon = createCustomIcon('#EF4444');   // Red
const blackIcon = createCustomIcon('#000000'); // Black

const center: [number, number] = [46.7712, 23.6236]; // Cluj-Napoca

interface MapComponentProps {
    bins: Bin[];
}

const MapComponent: React.FC<MapComponentProps> = ({ bins }) => {
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
                // Black: Last updated more than 1 hour ago (Not working)
                // Red: Fill level > 80% (and working)
                // Green: Otherwise (Working and < 80%)
                let icon = greenIcon;
                let statusColor = '#047857'; // Green text

                const lastUpdatedDate = new Date(bin.lastUpdated);
                const oneHourAgo = new Date(Date.now() - 60 * 60 * 1000);

                if (lastUpdatedDate < oneHourAgo) {
                    icon = blackIcon;
                    statusColor = '#1F2937'; // Gray-800
                } else if (bin.fillLevel > 80) {
                    icon = redIcon;
                    statusColor = '#B91C1C'; // Red text
                }

                return (
                    <Marker key={bin.id} position={[bin.lat, bin.lng]} icon={icon}>
                        <Popup>
                            <div style={{ color: '#333', textAlign: 'center', minWidth: '120px', padding: '5px' }}>

                                {/* Large Percentage Number */}
                                <div style={{
                                    fontSize: '1.3rem',
                                    fontWeight: '800',
                                    color: statusColor,
                                    marginBottom: '4px',
                                    lineHeight: '1',
                                    letterSpacing: '-1px'
                                }}>
                                    {bin.fillLevel}%
                                </div>

                                {/* Horizontal Progress Bar Container */}
                                <div style={{
                                    height: '8px',
                                    width: '100%',
                                    backgroundColor: '#e5e7eb',
                                    borderRadius: '999px',
                                    overflow: 'hidden',
                                    marginBottom: '6px',
                                    boxShadow: 'inset 0 1px 2px rgba(0,0,0,0.1)'
                                }}>
                                    {/* Progress Bar Fill */}
                                    <div style={{
                                        height: '100%',
                                        width: `${Math.min(bin.fillLevel, 100)}%`,
                                        background: `linear-gradient(90deg, ${statusColor}, ${statusColor}dd)`,
                                        borderRadius: '999px',
                                        transition: 'width 0.5s cubic-bezier(0.4, 0, 0.2, 1)',
                                        boxShadow: '0 1px 2px rgba(0,0,0,0.1)'
                                    }} />
                                </div>

                                <div style={{
                                    fontSize: '0.85rem',
                                    color: '#6b7280',
                                    fontWeight: '600'
                                }}>
                                    Fill Level
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
