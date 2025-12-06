export interface TrashCan {
    id: number;
    lat: number;
    lng: number;
    locationName: string;
    fillLevel: number; // 0-100
    status: 'active' | 'maintenance' | 'offline';
    lastFull: string;
}

export const mockTrashCans: TrashCan[] = [
    {
        id: 1,
        lat: 46.7712,
        lng: 23.6236,
        locationName: 'Piata Unirii',
        fillLevel: 85,
        status: 'active',
        lastFull: '10 mins ago'
    },
    {
        id: 2,
        lat: 46.7693,
        lng: 23.5898,
        locationName: 'Central Park',
        fillLevel: 45,
        status: 'active',
        lastFull: '2 hours ago'
    },
    {
        id: 3,
        lat: 46.7534,
        lng: 23.5956,
        locationName: 'Iulius Mall',
        fillLevel: 95,
        status: 'active',
        lastFull: '5 mins ago'
    },
    {
        id: 4,
        lat: 46.7765,
        lng: 23.6050,
        locationName: 'The Office',
        fillLevel: 15,
        status: 'active',
        lastFull: '1 day ago'
    },
    {
        id: 5,
        lat: 46.7561,
        lng: 23.5511,
        locationName: 'Vivo Center',
        fillLevel: 60,
        status: 'maintenance',
        lastFull: '3 hours ago'
    }
];
