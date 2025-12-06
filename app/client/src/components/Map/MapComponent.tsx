import React from 'react';
import { MapContainer, TileLayer, Marker, Popup } from 'react-leaflet';
import 'leaflet/dist/leaflet.css';
import { mockTrashCans } from '../../data/mockData';
import L from 'leaflet';

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

const center: [number, number] = [46.7712, 23.6236]; // Cluj-Napoca

const MapComponent: React.FC = () => {
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
            {mockTrashCans.map((can) => {
                // Logic for marker color
                const icon = can.fillLevel < 80 ? greenIcon : redIcon;

                return (
                    <Marker key={can.id} position={[can.lat, can.lng]} icon={icon}>
                        <Popup>
                            <div style={{ color: '#333', textAlign: 'center' }}>
                                <strong style={{ fontSize: '1.1em' }}>{can.locationName}</strong><br />
                                <div style={{
                                    marginTop: '5px',
                                    padding: '4px 8px',
                                    borderRadius: '4px',
                                    background: can.fillLevel < 80 ? '#ECFDF5' : '#FEF2F2',
                                    color: can.fillLevel < 80 ? '#047857' : '#B91C1C',
                                    fontWeight: 'bold'
                                }}>
                                    Fill Level: {can.fillLevel}%
                                </div>
                                <span style={{ fontSize: '0.9em', color: '#666' }}>Status: {can.status}</span>
                            </div>
                        </Popup>
                    </Marker>
                );
            })}
        </MapContainer>
    );
};

export default MapComponent;
