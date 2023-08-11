import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceDetailDialogComponent } from './device-detail-dialog.component';

describe('DeviceDetailDialogComponent', () => {
  let component: DeviceDetailDialogComponent;
  let fixture: ComponentFixture<DeviceDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DeviceDetailDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DeviceDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
