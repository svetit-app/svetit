import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DeviceItemGroupDetailDialogComponent } from './device-item-group-detail-dialog.component';

describe('DeviceItemGroupDetailDialogComponent', () => {
  let component: DeviceItemGroupDetailDialogComponent;
  let fixture: ComponentFixture<DeviceItemGroupDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DeviceItemGroupDetailDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DeviceItemGroupDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
