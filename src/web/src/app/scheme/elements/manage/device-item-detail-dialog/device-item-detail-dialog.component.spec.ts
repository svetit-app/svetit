import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceItemDetailDialogComponent } from './device-item-detail-dialog.component';

describe('DeviceItemDetailDialogComponent', () => {
  let component: DeviceItemDetailDialogComponent;
  let fixture: ComponentFixture<DeviceItemDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DeviceItemDetailDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DeviceItemDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
